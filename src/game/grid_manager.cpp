#include "game/grid_manager.h"

void Grid::init(int size, int step, size_t objectCount) { 
    GridRenderer::init(size, step, objectCount); 
}
void Grid::terminate() { GridRenderer::terminate(); }

void Grid::draw(
const std::vector<glm::vec3> positions, const std::vector<float> masses,
const glm::mat4 &view, const glm::mat4 &projection) {
    
    GridRenderer::draw({positions, masses}, view, projection);
}