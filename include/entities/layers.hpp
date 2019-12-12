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
    // Gadgets
    Gadgets = 4,
    // The base on which the UI is shown (e.g. the white UI background in the pause menu).
    UIBackground = 5,
    // The UI (e.g. buttons)
    UI = 6,
    // UI element that have to be in front of UI elements (e.g. text on a button).
    UIForeground = 7,
    // FPS counter and debugging layer
    Overlay = 8
};

#endif // FILE_LAYERS_HPP