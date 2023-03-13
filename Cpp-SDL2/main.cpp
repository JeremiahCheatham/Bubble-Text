#include "game.h"

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}