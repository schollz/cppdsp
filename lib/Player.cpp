#include "Player.h"

#include <algorithm>

void Player::trigger(std::unique_ptr<Synth> synth) {
  activeSynths.push_back(std::move(synth));
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

  activeSynths.erase(std::remove_if(activeSynths.begin(), activeSynths.end(),
                                    [](const std::unique_ptr<Synth>& s) {
                                      return s->isDone();
                                    }),
                     activeSynths.end());
}

int Player::getActiveSynthCount() const { return activeSynths.size(); }