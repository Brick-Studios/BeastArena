#include "brickengine/engine.hpp"

int main(int argc, char** argv) {
    auto engine = BrickEngine();
    engine.OpenWindow();
    while(true) {
    }
    engine.CloseWindow();
    return 0;
}
