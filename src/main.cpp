#include "Game.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  Game game;
  try {
    game.init();
    game.loop();
  } catch (GameException &e) {
    std::cerr << "Got an exception, exiting: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
