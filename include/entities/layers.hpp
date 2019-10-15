#ifndef FILE_LAYERS_HPP
#define FILE_LAYERS_HPP

enum class Layers {
    // The background img
    Background = 0,
    // Trees and critters and random pasable objects
    Middleground = 1,
    // Player and platforms
    Foreground = 2,
    // The actual UI
    UI = 3,
    // FPS counter and debugging layer
    Overlay = 4
};

#endif // FILE_LAYERS_HPP