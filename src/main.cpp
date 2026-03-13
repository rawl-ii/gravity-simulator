#include "game/scene.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Use: " << argv[0] << " <path_to_json>" << '\n';
        return 1;
    }

    std::string path = argv[1];
    scene::init(path);

    scene::run();
    scene::terminate();

    return 0;
}