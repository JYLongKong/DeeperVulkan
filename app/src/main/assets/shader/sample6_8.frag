#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals { // 一致变量块
    float brightFactor;// 亮度调节系数
} myBufferVals;

layout (binding = 1) uniform sampler2D tex;// 纹理采样器，代表一幅纹理
layout (location = 0) out vec4 outColor;// 输出到管线的片元颜色

void main() {
    vec2 texCoor = gl_PointCoord;// 从内建变量获取纹理坐标
    outColor = myBufferVals.brightFactor * textureLod(tex, texCoor, 0.0);// 计算片元最终颜色值
}
