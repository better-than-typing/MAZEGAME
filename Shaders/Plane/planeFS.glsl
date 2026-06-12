#version 330 core

in vec3 planeColor;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture0;

void main() {
    vec4 texColor = texture(texture0, TexCoords);
    FragColor = texture(texture0, TexCoords) * vec4(planeColor, 0.0);
}