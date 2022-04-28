#include "tool.hpp"

namespace tr3d {

std::vector<char> ReadFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        printf("file %s don't exists\n", filename);
        return std::vector<char>(); 
    }
    std::vector<char> result((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
    file.close();
    return result;
}

}
