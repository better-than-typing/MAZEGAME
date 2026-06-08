#version 330 core

in vec3 planeColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(planeColor, 1.0);
}