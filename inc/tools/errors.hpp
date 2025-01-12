#ifndef ARAMUTILS_ERROR_HPP
#define ARAMUTILS_ERROR_HPP

#include <rogue.hpp>

#include <exception>
#include <filesystem>
#include <stdexcept>
#include <string>

namespace aramutils {
    inline void checkPath(const std::filesystem::path &path) {
        if(!std::filesystem::exists(path)) {
            throw std::runtime_error(std::string("File not found:\n") + std::filesystem::absolute(path).string());
        }
    }
}

#endif