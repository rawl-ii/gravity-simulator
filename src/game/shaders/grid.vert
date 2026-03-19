#version 330 core
#define MAX_OBJECTS 256
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct object {
    vec3 position;
    float mass;
};
uniform int objectCount;
uniform object objects[MAX_OBJECTS];

uniform float softening;
uniform float intensity;
uniform float decay;

float schwarzschildWell(float radius, float mass) {
    return intensity * mass / pow(radius * radius + softening * softening, decay);
}

void main() {
    vec3 position = aPos;
    float yOffset = 0.0;

    for(int i = 0; i < objectCount; i++) {
        vec3 objXZ = vec3(objects[i].position.x, 0.0, objects[i].position.z);
        vec3 posXZ = vec3(position.x, 0.0, position.z);

        float radius = length(objXZ - posXZ);

        float pull = schwarzschildWell(radius, objects[i].mass);

        yOffset += pull;
    }

    position.y -= yOffset;

    gl_Position = projection * view * model * vec4(position, 1.0);
}