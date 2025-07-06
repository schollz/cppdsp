#include "Player.h"

#include <algorithm>
#include <iostream>

#include "KickSynth.h"

void Player::trigger(std::unique_ptr<Synth> synth) {
  activeSynths.push_back(std::move(synth));
}

void Player::triggerKick(std::unique_ptr<KickSynth> kickSynth) {
  // Fade out any existing KickSynths
  for (auto& synth : activeSynths) {
    KickSynth* kickPtr = dynamic_cast<KickSynth*>(synth.get());
    if (kickPtr) {
      kickPtr->fadeOutAndFree();
    }
  }

  // Add the new KickSynth
  activeSynths.push_back(std::move(kickSynth));
}

void Player::process(float** out, int blockSize) {
  for (auto& synth : activeSynths) {
    synth->process(out, blockSize);
  }

  // Apply DC block to both channels
  for (int i = 0; i < blockSize; i++) {
    out[0][i] = dcBlockLeft.process(out[0][i]);
    out[1][i] = dcBlockRight.process(out[1][i]);
  }

  for (auto& synth : activeSynths) {
    if (synth->isDone()) {
      // std::cerr << "Synth done\n";
    }
  }
  activeSynths.erase(std::remove_if(activeSynths.begin(), activeSynths.end(),
                                    [](const std::unique_ptr<Synth>& s) {
                                      return s->isDone();
                                    }),
                     activeSynths.end());
}

int Player::getActiveSynthCount() const { return activeSynths.size(); }