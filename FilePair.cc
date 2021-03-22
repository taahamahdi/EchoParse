#include "FilePair.h"
#include <filesystem>
#include <string>

using namespace std;

void FilePair::addFile(filesystem::path file, FilePairType type) {
    if (type == FilePairType::PRE) {
        this->pre = file;
    } else {
        this->post = file;
    }
}
