#version 330 core
#define MAX_POINT_LIGHTS 12

out vec4 fragColor;
in vec3 fragPos;
in vec3 normal;

struct light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;

    float constant;
    float linear;
    float quadratic;
}

uniform light pointLights[MAX_POINT_LIGHTS];
uniform vec3 viewerPos;
uniform vec3 color;

vec3 calculatePointLight(light pointLight, vec3 norm) {
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float lightDist = length(pointLight.position - fragPos);

    vec3 normalizedNormal = normalize(norm);
    float diff = max(dot(normalizedNormal, lightDir), 0.0);

    vec3 ambient = pointLight.ambient * color;
    vec3 diffuse =  pointLight.diffuse * diff * color;

    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * lightDist + pointLight.quadratic * (lightDist * lightDist));

    return ambient + diffuse * attenuation;
}

void main() {
    vec3 normalizedNormal = normalize(normal);

    vec3 result = vec3(0.0);
    for(int i = 0; i < MAX_POINT_LIGHTS; i++) {
        result += calculatePointLight(pointLights[i], normalizedNormal);
    }

    fragColor = vec4(result, 1.0);
}