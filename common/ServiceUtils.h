#ifndef SERVICE_UTILS_H
#define SERVICE_UTILS_H

#include <string>

inline std::string buildValidationError(bool valid, const std::string& message) {
    if (!valid) {
        return "- " + message + "\n";
    }
    return "";
}

inline std::string wrapAddErrors(const std::string& entityName, const std::string& errors) {
    return entityName + " cannot be added:\n" + errors;
}

inline std::string wrapUpdateErrors(const std::string& entityName, const std::string& errors) {
    return entityName + " cannot be updated:\n" + errors;
}

#endif
