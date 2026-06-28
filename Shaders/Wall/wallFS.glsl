#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 wallColor;
uniform sampler2D texture0;


float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {

    //float depth = LinearizeDepth(gl_FragCoord.z) / far;
    //FragColor = vec4(vec3(depth), 1.0);
    vec4 texColor = texture(texture0, TexCoords);


    if(texColor.a < 0.1)
    discard;

    FragColor = texColor;
}