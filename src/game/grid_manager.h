//TODO: a grid
#include "engine/grid_renderer.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class Grid {
public:
    static void init(int size, int step, size_t objectCount);
    static void terminate();
    
    static void draw(
        const std::vector<glm::vec3> positions, const std::vector<float> masses,
        const glm::mat4 &view, const glm::mat4 &projection
    );
};