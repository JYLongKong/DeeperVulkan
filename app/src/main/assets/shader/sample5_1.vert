#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform constantVals { // 一致块
    mat4 mvp;// 最终变换矩阵
} myConstantVals;

layout (location = 0) in vec3 pos;// 传入的顶点位置
layout (location = 0) out vec3 vposition;// 传输到片元着色器的顶点位置

out gl_PerVertex { // 输出接口块
    vec4 gl_Position;// 顶点最终位置
};

void main() {
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);// 计算顶点最终位置
    vposition=pos;// 把顶点位置传给片元着色器
}