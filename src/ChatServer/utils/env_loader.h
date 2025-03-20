#ifndef ENV_LOADER_H
#define ENV_LOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

class EnvLoader {
public:
    static void loadEnv(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open .env file");
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '=')) {
                std::string value;
                if (std::getline(is_line, value)) {
                    #ifdef _WIN32
                        _putenv_s(key.c_str(), value.c_str());
                    #else
                        setenv(key.c_str(), value.c_str(), 1);
                    #endif
                }
            }
        }
    }
};

#endif // ENV_LOADER_H
