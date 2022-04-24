#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;// 摄像机位置
    vec4 uLightDirection;// 光源方向向量
    vec4 lightAmbient;
    vec4 lightDiffuse;
    vec4 lightSpecular;
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

// 定向光光照计算的方法
vec4 directionalLight(
in mat4 uMMatrix, // 基本变换矩阵
in vec3 uCamera, // 摄像机位置
in vec3 lightDirection, // 定向光方向
in vec4 lightAmbient, // 环境光强度
in vec4 lightDiffuse, // 散射光强度
in vec4 lightSpecular, // 镜面光强度
in vec3 normal, // 法向量
in vec3 aPosition// 顶点位置
) {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    ambient = lightAmbient;

    vec3 newNormal = normalize((uMMatrix * vec4(normal, 0)).xyz);
    vec3 vp = normalize(lightDirection);// 格式化定向光方向向量
    float nDotViewPosition = max(0.0, dot(newNormal, vp));
    diffuse = lightDiffuse * nDotViewPosition;

    float shininess = 50.0;
    vec3 eye = normalize(uCamera - (uMMatrix * vec4(aPosition, 1)).xyz);
    vec3 halfVector = normalize(vp + eye);// 求视线与光线向量的半向量
    float nDotViewHalfVector = dot(newNormal, halfVector);
    float powerFactor = max(0.0, pow(nDotViewHalfVector, shininess));
    specular = lightSpecular * powerFactor;

    return ambient + diffuse + specular;
}

void main() {
    outLightQD = directionalLight(
    myConstantVals.mm,
    myBufferVals.uCamera.xyz,
    myBufferVals.uLightDirection.xyz,
    myBufferVals.lightAmbient,
    myBufferVals.lightDiffuse,
    myBufferVals.lightSpecular,
    inNormal,
    pos
    );
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);
    vposition = pos;
}
