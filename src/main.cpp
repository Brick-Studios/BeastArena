#include "brickengine/engine.hpp"

int main(int argc, char** argv) {
    auto engine = BrickEngine();
    engine.Start("Beast Arena");
    while(true) {
    }
    engine.Shutdown();
    return 0;
}
