#include "library.hpp"

// Ensure that the dirpath exits if not create it
bool createDirectory(const std::string &dirPath)
{
    struct stat st;
    if (stat(dirPath.c_str(), &st) == -1)
    {
        if (mkdir(dirPath.c_str(), 0700) == -1)
        {
            PrintLog(std::cerr, RED "Failed to create %s: %s" RESET,
                     dirPath.c_str(), strerror(errno));
            return false;
        }
    }
    return true;
}
