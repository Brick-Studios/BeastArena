#ifndef FILE_READY_COMPONENT_HPP
#define FILE_READY_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class ReadyComponent : public ComponentImpl<ReadyComponent> {
public:
    static std::string getNameStatic();
    // Data
    bool readied_up = false;
};

#endif // FILE_READY_COMPONENT_HPP