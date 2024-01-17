#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 uMatrix;
uniform vec2 uLightPos;

void main() {
    vec2 position = (uMatrix * vec4(aPos.xy, 0.0, 1.0)).xy;
    gl_Position = vec4(position - aPos.z * uLightPos, 0, 1.0 - aPos.z);
}