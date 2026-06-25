#version 330 core

in vec3 planeColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture0;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 calculateLighting() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    //TODO Make this a slider in gui
    float ambientStrength = 0.01;
    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    //TODO Make this a slider in gui
    float specularStrength = 0.2f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //TODO Make this a slider in gui
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64);
    vec3 specular = spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * planeColor;

    return result;
}

void main() {

    vec4 texColor = texture(texture0, TexCoords);

    // Is Gouraund More Better?
    FragColor = texture(texture0, TexCoords) * vec4(calculateLighting(), 0.0);
}