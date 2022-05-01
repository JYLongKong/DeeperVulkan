#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform constantVals {
    mat4 mvp;
    mat4 mm;
} myConstantVals;

layout (location = 0) in vec3 pos;// 传入的顶点坐标
layout (location = 1) in vec3 inNormal;// 传入的顶点法向量
layout (location = 1) out vec3 vposition;// 传出的顶点坐标
layout (location = 2) out vec3 vNormal;// 传出的世界坐标系法向量
layout (location = 3) out vec3 objPos;// 传出的物体坐标系顶点坐标

out gl_PerVertex {
    vec4 gl_Position;
};

// 将物体坐标系的法向量变换到世界坐标系的方法
vec3 normalFromObjectToWorld(
in mat4 uMMatrix, // 基本变换矩阵
in vec3 normal, // 要变换的法向量
in vec3 position// 顶点位置
) {
    vec3 normalTarget = position + normal;// 计算变换后的法向量
    vec3 newNormal = (uMMatrix * vec4(normalTarget, 1)).xyz - (uMMatrix * vec4(position, 1)).xyz;
    newNormal = normalize(newNormal);
    return newNormal;
}

void main() {
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);// 计算顶点最终位置
    vposition = (myConstantVals.mm * vec4(pos, 1)).xyz;// 计算世界坐标系顶点位置
    vNormal = normalFromObjectToWorld(myConstantVals.mm, inNormal, pos);// 计算世界坐标系法向量
    objPos = pos;// 传送到片元着色器的物体坐标系顶点位置
}
