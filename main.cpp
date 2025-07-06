#include <jack/jack.h>
#include <signal.h>
#include <sndfile.h>

#include <cmath>
#include <cstring>
#include <iostream>

#include "lib/KickSynth.h"
#include "lib/Player.h"

Player* g_player = nullptr;
double g_sampleRate = 44100.0;
double g_bpm = 150.0;
double g_samplesPerBeat = 0.0;
int g_sampleCount = 0;
int g_nextBeatSample = 0;
int g_beatCount = 0;
bool g_running = true;
SNDFILE* g_wavFile = nullptr;
SF_INFO g_wavInfo;

void signal_handler(int sig) { g_running = false; }

int jack_process(jack_nframes_t nframes, void* arg) {
  if (!g_player || !g_running) return 0;

  jack_default_audio_sample_t* out_left =
      (jack_default_audio_sample_t*)jack_port_get_buffer(
          (jack_port_t*)((void**)arg)[0], nframes);
  jack_default_audio_sample_t* out_right =
      (jack_default_audio_sample_t*)jack_port_get_buffer(
          (jack_port_t*)((void**)arg)[1], nframes);

  if (g_sampleCount >= g_nextBeatSample) {
    auto kick = std::make_unique<KickSynth>(g_sampleRate);
    g_player->trigger(std::move(kick));
    g_nextBeatSample += static_cast<int>(g_samplesPerBeat);
    g_beatCount++;
    std::cout << "Beat " << g_beatCount << " at sample " << g_sampleCount
              << std::endl;
  }

  std::memset(out_left, 0, nframes * sizeof(jack_default_audio_sample_t));
  std::memset(out_right, 0, nframes * sizeof(jack_default_audio_sample_t));

  float* audioBuffer[2];
  audioBuffer[0] = (float*)out_left;
  audioBuffer[1] = (float*)out_right;

  g_player->process(audioBuffer, nframes);

  // Write to WAV file
  if (g_wavFile) {
    float interleavedBuffer[nframes * 2];
    for (int i = 0; i < nframes; i++) {
      interleavedBuffer[i * 2] = audioBuffer[0][i];
      interleavedBuffer[i * 2 + 1] = audioBuffer[1][i];
    }
    sf_write_float(g_wavFile, interleavedBuffer, nframes * 2);
  }

  // Debug: Check for non-zero audio
  static int debug_count = 0;
  if (debug_count++ < 10) {
    float max_val = 0.0f;
    for (int i = 0; i < nframes; i++) {
      if (std::abs(audioBuffer[0][i]) > max_val)
        max_val = std::abs(audioBuffer[0][i]);
    }
    if (max_val > 0.0f) {
      std::cout << "Audio generated: max=" << max_val << std::endl;
    }
  }

  g_sampleCount += nframes;

  return 0;
}

void jack_shutdown(void* arg) { 
  g_running = false; 
  if (g_wavFile) {
    sf_close(g_wavFile);
    g_wavFile = nullptr;
  }
}

int main() {
  jack_client_t* client;
  jack_port_t* output_port_left;
  jack_port_t* output_port_right;
  void* ports[2];

  signal(SIGINT, signal_handler);

  // Open WAV file for writing
  g_wavInfo.samplerate = 44100;
  g_wavInfo.channels = 2;
  g_wavInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
  g_wavFile = sf_open("output.wav", SFM_WRITE, &g_wavInfo);
  if (!g_wavFile) {
    std::cerr << "Cannot open output.wav for writing" << std::endl;
    return 1;
  }

  client = jack_client_open("drumsynth", JackNullOption, NULL);
  if (!client) {
    std::cerr << "Cannot open JACK client" << std::endl;
    return 1;
  }

  g_sampleRate = jack_get_sample_rate(client);
  g_samplesPerBeat = g_sampleRate / (g_bpm / 60.0);

  Player player;
  g_player = &player;

  output_port_left = jack_port_register(
      client, "output_left", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
  output_port_right = jack_port_register(
      client, "output_right", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

  if (!output_port_left || !output_port_right) {
    std::cerr << "Cannot register output ports" << std::endl;
    return 1;
  }

  ports[0] = output_port_left;
  ports[1] = output_port_right;

  jack_set_process_callback(client, jack_process, ports);
  jack_on_shutdown(client, jack_shutdown, NULL);

  if (jack_activate(client)) {
    std::cerr << "Cannot activate JACK client" << std::endl;
    return 1;
  }

  // Auto-connect to system playback ports
  if (jack_connect(client, "drumsynth:output_left", "system:playback_1")) {
    std::cerr << "Cannot connect output_left to system:playback_1" << std::endl;
  }
  if (jack_connect(client, "drumsynth:output_right", "system:playback_2")) {
    std::cerr << "Cannot connect output_right to system:playback_2" << std::endl;
  }

  std::cout << "JACK client activated" << std::endl;
  std::cout << "Playing kick drum at " << g_bpm << " BPM" << std::endl;
  std::cout << "Sample rate: " << g_sampleRate << " Hz" << std::endl;
  std::cout << "Recording to output.wav" << std::endl;
  std::cout << "Auto-connected to system playback ports" << std::endl;
  std::cout << "Press Ctrl+C to stop" << std::endl;

  while (g_running) {
    usleep(100000);
  }

  if (g_wavFile) {
    sf_close(g_wavFile);
    g_wavFile = nullptr;
    std::cout << "WAV file saved as output.wav" << std::endl;
  }

  jack_client_close(client);

  return 0;
}