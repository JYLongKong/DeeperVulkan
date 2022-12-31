#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 1) uniform sampler2D tex; // Sample7_4

layout (location = 0) in vec2 inTexCoor;    // Sample7_4
layout (location = 1) in vec4 inLightQD;
layout (location = 0) out vec4 outColor;

void main() {
    // outColor = inLightQD * vec4(0.9, 0.9, 0.9, 1.0);
    outColor = inLightQD * textureLod(tex, inTexCoor, 0.0); // Sample7_4
}
