#ifndef FILE_PAIR
#define FILE_PAIR

#include <filesystem>
#include <string>

enum FilePairType {
    PRE = 0,
    POST = 1,
};

struct FilePair {
    std::filesystem::path pre;
    std::filesystem::path post;

    void addFile(std::filesystem::path file, FilePairType type);
};

#endif
