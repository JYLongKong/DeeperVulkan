#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform constantVals {
    mat4 mvp;
    float pointSize;// 点的大小
} myConstantVals;

layout (location = 0) in vec3 pos;// 输入的顶点位置
layout (location = 1) in vec3 color;// 输入的顶点颜色
layout (location = 0) out vec3 vcolor;// 传到片元着色器的顶点颜色

out gl_PerVertex {
    vec4 gl_Position;// 内建变量gl_Position
    float gl_PointSize;// 内建变量gl_PointSize
};

void main() {
    gl_PointSize = myConstantVals.pointSize;// 点绘制方式时点的大小
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);
    vcolor = color;
}
