#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;
    vec4 lightPosition;
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
layout (location = 0) out vec4 outLightQD;// 输出到片元着色器的正面光照强度
layout (location = 1) out vec4 outLightQDBack;// 输出到片元着色器的反面光照强度

out gl_PerVertex {
    vec4 gl_Position;
};

vec4 pointLight(
    in mat4 uMMatrix,
    in vec3 uCamera,
    in vec3 lightLocation,
    in vec4 lightAmbient,
    in vec4 lightDiffuse,
    in vec4 lightSpecular,
    in vec3 normal,
    in vec3 aPosition
) {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    ambient = lightAmbient;
    vec3 normalTarget = aPosition + normal;
    vec3 newNormal = (uMMatrix * vec4(normalTarget, 1)).xyz - (uMMatrix * vec4(aPosition, 1)).xyz;
    newNormal = normalize(newNormal);
    vec3 eye = normalize(lightLocation - (uMMatrix * vec4(aPosition, 1)).xyz);
    vec3 vp = normalize(lightLocation - (uMMatrix * vec4(aPosition, 1)).xyz);
    vp = normalize(vp);
    vec3 halfVector = normalize(vp + eye);
    float shininess = 50.0;
    float nDotViewPosition = max(0.0, dot(newNormal, vp));
    diffuse = lightDiffuse * nDotViewPosition;
    float nDotViewHalfVector = dot(newNormal, halfVector);
    float powerFactor = max(0.0, pow(nDotViewHalfVector, shininess));
    specular = lightSpecular * powerFactor;
    return ambient + diffuse + specular;
}

void main() {
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
    outLightQDBack = pointLight(
        myConstantVals.mm,
        myBufferVals.uCamera.xyz,
        myBufferVals.lightPosition.xyz,
        myBufferVals.lightAmbient,
        myBufferVals.lightDiffuse,
        myBufferVals.lightSpecular,
        -inNormal,  // 法向量(反面的)
        pos
    );
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);
}
