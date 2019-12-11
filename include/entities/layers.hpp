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
    // The base on which the UI is shown (e.g. the white UI background in the pause menu).
    UIBackground = 4,
    // The UI (e.g. buttons)
    UI = 5,
    // UI element that have to be in front of UI elements (e.g. text on a button).
    UIForeground = 6,
    // FPS counter and debugging layer
    Overlay = 7
};

#endif // FILE_LAYERS_HPP