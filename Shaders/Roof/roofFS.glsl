#version 330 core

in vec3 planeColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture0;

void main() {

    vec4 texColor = texture(texture0, TexCoords);

    // Is Gouraund More Better?
    FragColor = texColor;
}