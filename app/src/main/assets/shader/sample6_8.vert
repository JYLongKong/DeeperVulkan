#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform constantVals {
    mat4 mvp;
} myConstantVals;

layout (location = 0) in vec3 pos;

out gl_PerVertex { // 输出接口块
    vec4 gl_Position;// 内建变量gl_Position
    float gl_PointSize;// 内建变量gl_PointSize
};

void main() {
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);
    gl_PointSize = 64;// 设置点精灵对应点的尺寸
}
