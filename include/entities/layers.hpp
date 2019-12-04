#ifndef FILE_LAYERS_HPP
#define FILE_LAYERS_HPP

enum class Layers {
    // The background img
    Background = 0,
    // Special low layer
    Lowground = 1,
    // Trees and critters and random pasable objects
    Middleground = 2,
    // Player and platforms
    Foreground = 3,
    // The actual UI
    UI = 4,
    // FPS counter and debugging layer
    Overlay = 5
};

#endif // FILE_LAYERS_HPP