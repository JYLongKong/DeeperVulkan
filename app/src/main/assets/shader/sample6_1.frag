#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals {
    float brightFactor;// 亮度调节系数
} myBufferVals;

layout (binding = 1) uniform sampler2D tex;// 纹理采样器，代表一幅纹理
layout (location = 0) in vec2 inTexCoor;// 接收的顶点纹理坐标
layout (location = 0) out vec4 outColor;// 输出到管线的片元颜色

void main() {
    outColor = myBufferVals.brightFactor * textureLod(tex, inTexCoor, 0.0); // 计算最终颜色值
}
