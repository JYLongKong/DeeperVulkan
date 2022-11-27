#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals {
    float brightFactor;
} myBufferVals;

layout (binding = 1) uniform sampler2DArray tex;
layout (location = 0) in vec3 inTexCoor;
layout (location = 0) out vec4 outColor;

void main() {
    outColor = myBufferVals.brightFactor * textureLod(tex, inTexCoor, 0.0);
}
