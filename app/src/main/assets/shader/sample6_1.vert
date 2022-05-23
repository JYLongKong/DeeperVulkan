#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform constantVals {
    mat4 mvp;
} myConstantVals;

layout (location = 0) in vec3 pos;// 输入的顶点位置
layout (location = 1) in vec2 inTexCoor;// 输入的顶点纹理坐标
layout (location = 0) out vec2 outTexCoor;// 用于传递给片元着色器的纹理坐标

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    outTexCoor = inTexCoor;// 将接收的纹理坐标传递给片元着色器
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);// 计算顶点最终位置
}
