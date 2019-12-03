#ifndef FILE_GADGET_TYPE_HPP
#define FILE_GADGET_TYPE_HPP

#include "brickengine/exceptions/not_implemented_exception.hpp"

enum class GadgetType {
    Pistol,
    Rifle,
    Sniper
};

class GadgetTypeConverter {
public:
    static GadgetType stringToGadgetType(std::string s) {
        if (s == "pistol" || s == "Pistol") {
            return GadgetType::Pistol;
        } else if (s == "rifle" || s == "Rifle") {
            return GadgetType::Rifle;
        } else if (s == "sniper" || s == "Sniper") {
            return GadgetType::Sniper;
        } else {
            throw NotImplementedException("GadgetType: " + s + " not implemented in stringToGadgetType.");
        }
    }
};

#endif // FILE_GADGET_TYPE_HPP