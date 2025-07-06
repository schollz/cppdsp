#include <sndfile.h>

#include <cmath>
#include <cstring>
#include <iostream>

#include "lib/KickSynth.h"
#include "lib/Player.h"

int main() {
  const double sampleRate = 44100.0;
  const double bpm = 150.0;
  const double samplesPerBeat = sampleRate / (bpm / 60.0);
  const int duration = 30;  // seconds
  const int totalSamples = duration * sampleRate;
  const int blockSize = 512;

  int sampleCount = 0;
  int nextBeatSample = 0;
  int beatCount = 0;

  // Open WAV file for writing
  SF_INFO wavInfo;
  wavInfo.samplerate = sampleRate;
  wavInfo.channels = 2;
  wavInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
  SNDFILE* wavFile = sf_open("output.wav", SFM_WRITE, &wavInfo);
  if (!wavFile) {
    std::cerr << "Cannot open output.wav for writing" << std::endl;
    return 1;
  }

  Player player;

  std::cout << "Generating " << duration << " seconds of audio" << std::endl;
  std::cout << "Kick drum at " << bpm << " BPM" << std::endl;
  std::cout << "Sample rate: " << sampleRate << " Hz" << std::endl;
  std::cout << "Writing to output.wav" << std::endl;

  // Process audio in blocks
  while (sampleCount < totalSamples) {
    int currentBlockSize = std::min(blockSize, totalSamples - sampleCount);

    // Check if we need to trigger a kick
    if (sampleCount >= nextBeatSample) {
      auto kick = std::make_unique<KickSynth>(sampleRate);
      player.triggerKick(std::move(kick));
      nextBeatSample += static_cast<int>(samplesPerBeat);
      beatCount++;
    }

    // Create audio buffers
    float leftBuffer[blockSize];
    float rightBuffer[blockSize];
    std::memset(leftBuffer, 0, currentBlockSize * sizeof(float));
    std::memset(rightBuffer, 0, currentBlockSize * sizeof(float));

    float* audioBuffer[2];
    audioBuffer[0] = leftBuffer;
    audioBuffer[1] = rightBuffer;

    // Process audio
    player.process(audioBuffer, currentBlockSize);

    // Write to WAV file
    float interleavedBuffer[currentBlockSize * 2];
    for (int i = 0; i < currentBlockSize; i++) {
      interleavedBuffer[i * 2] = audioBuffer[0][i];
      interleavedBuffer[i * 2 + 1] = audioBuffer[1][i];
    }
    sf_write_float(wavFile, interleavedBuffer, currentBlockSize * 2);

    sampleCount += currentBlockSize;
  }

  sf_close(wavFile);
  std::cout << "WAV file saved as output.wav (" << duration << " seconds)"
            << std::endl;

  return 0;
}