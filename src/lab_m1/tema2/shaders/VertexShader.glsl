#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec3 frag_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

float noise(vec2 st) {
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    vec2 coordPlan = vec2(in_position.x, in_position.z);
    float scale = 0.5f;
    float maxHeight = 2.0f;
    float vertexHeight = noise(coordPlan * scale) * maxHeight;

    vec3 newPosition = in_position;
    newPosition.y = newPosition.y + vertexHeight;

    vec3 green = vec3(0.1, 0.3, 0.1);
    vec3 orange = vec3(0.8, 0.4, 0.2);
    float heightFactor = vertexHeight / maxHeight;
    frag_color = mix(green, orange, heightFactor);

    gl_Position = projection * view * model * vec4(newPosition, 1.0);
}