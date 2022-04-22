#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;// 摄像机位置
    vec4 lightPosition;// 光源位置
    vec4 lightAmbient;// 环境光强度
    vec4 lightDiffuse;// 散射光强度
    vec4 lightSpecular;// 镜面光强度
} myBufferVals;

layout (push_constant) uniform constantVals {
    mat4 mvp;
    mat4 mm;
} myConstantVals;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
layout (location = 0) out vec4 outLightQD;
layout (location = 1) out vec3 vposition;

out gl_PerVertex {
    vec4 gl_Position;
};

// 定位光光照计算的方法
vec4 pointLight(
in mat4 uMMatrix, // 基本变换矩阵
in vec3 uCamera, // 摄像机位置
in vec3 lightLocation, // 光源位置
in vec4 lightAmbient, // 环境光强度
in vec4 lightDiffuse, // 散射光强度
in vec4 lightSpecular, // 镜面光强度
in vec3 normal, // 法向量
in vec3 aPosition// 顶点位置
) {
    vec4 ambient;// 环境光最终强度
    vec4 diffuse;// 散射光最终强度
    vec4 specular;// 镜面光最终强度

    ambient = lightAmbient;// 直接得出环境光的最终强度

    vec3 normalTarget = aPosition + normal;// 计算变换后的法向量
    vec3 newNormal = (uMMatrix * vec4(normalTarget, 1)).xyz - (uMMatrix * vec4(aPosition, 1)).xyz;
    newNormal = normalize(newNormal);// 对法向量规格化
    vec3 vp = normalize(lightLocation - (uMMatrix * vec4(aPosition, 1)).xyz);// 顶点到光源位置的向量vp
    vp = normalize(vp);// 格式化vp向量
    float nDotViewPosition = max(0.0, dot(newNormal, vp));// 求法向量与vp向量的点积与0的最大值
    diffuse = lightDiffuse * nDotViewPosition;// 计算散射光的最终强度

    float shininess = 50.0;// 粗糙度，越小越光滑
    vec3 eye = normalize(uCamera - (uMMatrix * vec4(aPosition, 1)).xyz);// 顶点到摄像机的向量
    vec3 halfVector = normalize(vp + eye);// 求视线与vp向量的半向量
    float nDotViewHalfVector = dot(newNormal, halfVector);// 法线与半向量的点积
    float powerFactor = max(0.0, pow(nDotViewHalfVector, shininess));// 镜面反射光强度因子
    specular = lightSpecular * powerFactor;// 计算镜面光的最终强度

    return ambient + diffuse + specular;// 将三个光照通道最终强度值求和返回
}

void main() {
    // 将最终光照强度传递给片元着色器
    outLightQD = pointLight(
    myConstantVals.mm,
    myBufferVals.uCamera.xyz,
    myBufferVals.lightPosition.xyz,
    myBufferVals.lightAmbient,
    myBufferVals.lightDiffuse,
    myBufferVals.lightSpecular,
    inNormal,
    pos
    );
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);
    vposition = pos;
}
