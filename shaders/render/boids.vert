#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aInstancePos;
layout (location = 4) in vec3 aInstanceVel;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 v_color;

void main() {
    vec3 forward = normalize(aInstanceVel);

    // Pick a fallback up vector if forward is almost parallel to world up
    vec3 worldUp = abs(forward.y) > 0.999 ? vec3(1.0, 0.0, 0.0)
                                          : vec3(0.0, 1.0, 0.0);

    vec3 right = normalize(cross(worldUp, forward));
    vec3 up    = cross(forward, right);

    mat3 rot = mat3(right, up, forward);

    vec3 rotatedPos = rot * aPos;
    vec3 worldPos = rotatedPos + aInstancePos;

    gl_Position = projection * view * model * vec4(worldPos, 1.0);
    v_color = (aInstanceVel + vec3(1.0)) * 0.5;
}

