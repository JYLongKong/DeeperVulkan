#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals { // 一致块
    vec4 uCamera;// 摄像机位置
    vec4 lightPosition;// 光源位置
    vec4 lightSpecular;// 镜面光强度
} myBufferVals;

layout (push_constant) uniform constantVals { // 推送常量块
    mat4 mvp;// 最终变换矩阵
    mat4 mm;// 基本变换矩阵
} myConstantVals;

layout (location = 0) in vec3 pos;// 输入的顶点位置
layout (location = 1) in vec3 inNormal;// 输入的法向量
layout (location = 0) out vec4 outLightQD;// 输出的光照强度
layout (location = 1) out vec3 vposition;// 输出的顶点位置

out gl_PerVertex { // 输出接口块
    vec4 gl_Position;// 内建变量gl_Position
};

// 定位光光照计算的方法
vec4 pointLight(
in mat4 uMMatrix, // 基本变换矩阵
in vec3 uCamera, // 摄像机位置
in vec3 lightLocation, // 光源位置
in vec4 lightSpecular, // 镜面光强度
in vec3 normal, // 法向量
in vec3 aPosition// 顶点位置
) {
    vec4 specular;// 镜面光最终强度
    vec3 normalTarget = aPosition + normal;// 计算变换后的法向量
    vec3 newNormal = (uMMatrix * vec4(normalTarget, 1)).xyz - (uMMatrix * vec4(aPosition, 1)).xyz;
    newNormal = normalize(newNormal);// 对法向量规格化
    vec3 eye = normalize(uCamera - (uMMatrix * vec4(aPosition, 1)).xyz);// 计算表面点到摄像机的向量
    vec3 vp = normalize(lightLocation - (uMMatrix * vec4(aPosition, 1)).xyz);// 计算表面点到光源位置的向量vp
    vp = normalize(vp);// 格式化vp向量
    vec3 halfVector = normalize(vp + eye);// 求视线与vp向量的半向量
    float shininess = 50.0;// 粗糙度，越小越光滑
    float nDotViewHalfVector = dot(newNormal, halfVector);// 法线与半向量的点积
    float powerFactor = max(0.0, pow(nDotViewHalfVector, shininess));// 镜面反射光强度因子
    specular = lightSpecular * powerFactor;// 计算镜面光的最终强度

    return specular;// 返回镜面光最终强度
}

void main() {
    // 将镜面光最终强度传递给片元着色器
    outLightQD = pointLight(
    myConstantVals.mm,
    myBufferVals.uCamera.xyz,
    myBufferVals.lightPosition.xyz,
    myBufferVals.lightSpecular,
    inNormal,
    pos
    );
    // 计算顶点最终位置
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);
    // 传到片元着色器的顶点位置
    vposition = pos;
}
