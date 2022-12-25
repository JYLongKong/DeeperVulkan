#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals {
    float brightFactor;
} myBufferVals;

layout (location = 0) in vec3 vPosition;
layout (location = 0) out vec4 outColor;

void main() {
    vec4 bColor = vec4(0.678, 0.231, 0.129, 0);// 深红色
    vec4 mColor = vec4(0.763, 0.657, 0.614, 0);// 淡红色
    float y = vPosition.y;// 提取当前片元的y坐标值
    y = mod((y + 100.0) * 4.0, 4.0);// 折算出区间值
    if (y > 1.8) {
        outColor = bColor;
    } else {
        outColor = mColor;
    }
    outColor = myBufferVals.brightFactor * outColor;
}
