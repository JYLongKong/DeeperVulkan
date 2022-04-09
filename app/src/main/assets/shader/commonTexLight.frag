#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 vcolor;// 顶点着色器传入的顶点颜色数据
layout (location = 0) out vec4 outColor;// 输出到渲染管线的片元颜色值

void main() {
    outColor=vec4(vcolor.rgb, 1.0);// 将顶点着色器传递过来的颜色值输出
}
