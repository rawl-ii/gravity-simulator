#version 330 core
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;
uniform vec3 color;

void main() {
    vec3 normalizedNormal = normalize(normal);
    vec3 lightDir = normalize(viewPos - fragPos);

    float fresnelTerm = dot(normalizedNormal, lightDir);
    fresnelTerm = clamp(1.0 - fresnelTerm, 0.0, 1.0);

    float intensity = pow(fresnelTerm, 3.0);

    vec3 finalColor = color + (intensity * vec3(1.0, 1.0, 1.0));

    fragColor = vec4(finalColor, 1.0);
} 