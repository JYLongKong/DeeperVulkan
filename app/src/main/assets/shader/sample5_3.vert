#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(std140, set = 0, binding = 0) uniform bufferVals { // 一致块flushUniformBuffer()
    vec4 lightPosition;// 光源位置
    vec4 lightDiffuse;// 散射光强度
} myBufferVals;

layout(push_constant) uniform constantVals { // 推送常量块
    mat4 mvp;// 最终变换矩阵
    mat4 mm;// 基本变换矩阵
} myConstantVals;

layout(location = 0) in vec3 pos;// 传入的顶点位置
layout(location = 1) in vec3 inNormal;// 传入的顶点法向量
layout(location = 0) out vec4 outLightQD;// 传出的光照强度
layout(location = 1) out vec3 vposition;// 传出的顶点位置

out gl_PerVertex { // 输出接口块
    vec4 gl_Position;// 内建变量gl_Position
};

// 定位光光照计算的方法
vec4 pointLight(
in mat4 uMMatrix, // 基本变换矩阵
in vec3 lightLocation, // 光源位置
in vec4 lightDiffuse, // 散射光强度
in vec3 normal, // 法向量
in vec3 aPosition// 顶点位置
) {
    vec4 diffuse;// 散射光最终强度
    vec3 normalTarget = aPosition + normal;// 计算世界坐标系中的法向量
    vec3 newNormal = (uMMatrix * vec4(normalTarget, 1)).xyz - (uMMatrix * vec4(aPosition, 1)).xyz;
    newNormal = normalize(newNormal);// 对法向量规格化
    vec3 vp = normalize(lightLocation - (uMMatrix * vec4(aPosition, 1)).xyz);// 计算表面点到光源位置的向量
    vp = normalize(vp);// 格式化vp向量
    float nDotViewPosition = max(0.0, dot(newNormal, vp));// 求法向量与vp的点积与0的最大值
    diffuse = lightDiffuse * nDotViewPosition;// 计算散射光的最终强度

    return diffuse;// 返回散射光最终强度
}

void main() {
    // 将散射光最终强度传递给片元着色器
    outLightQD = pointLight(myConstantVals.mm, myBufferVals.lightPosition.xyz, myBufferVals.lightDiffuse, inNormal, pos);
    // 计算顶点最终位置
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);
    // 传递顶点位置给片元着色器
    vposition = pos;
}
