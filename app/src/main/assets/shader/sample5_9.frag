#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;
    vec4 lightPosition;
    vec4 lightAmbient;
    vec4 lightDiffuse;
    vec4 lightSpecular;
} myBufferVals;

layout (location = 1) in vec3 vposition;// 传入的世界坐标系顶点位置
layout (location = 2) in vec3 vNormal;// 传入的世界坐标系法向量
layout (location = 3) in vec3 objPos;// 传入的物体坐标系顶点位置
layout (location = 0) out vec4 outColor;// 输出到渲染管线的最终片元颜色

vec4 genBoardColor(vec3 position) {
    const float height = 4.0;
    const float width = 6.0;
    vec4 color;
    float n = 8.0;
    float spanh = height / n;
    float spanw = width / n;
    int i = int((position.x + 10.0) / spanw);
    int j = int((position.y + 10.0) / spanh);
    int whichColor = int(mod(float(i + j), 2.0));
    if (whichColor == 1) {
        color = vec4(0.678, 0.231, 0.129, 1.0);// 红色
    } else {
        color = vec4(1.0, 1.0, 1.0, 1.0);// 白色
    }

    return color;
}

vec4 pointLight(
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
    vec3 eye = normalize(uCamera - aPosition);
    vec3 vp = normalize(lightLocation.xyz - aPosition);
    vp = normalize(vp);
    vec3 halfVector = normalize(vp + eye);
    float shininess = 10.0;
    float nDotViewPosition = max(0.0, dot(normal, vp));
    diffuse = lightDiffuse * nDotViewPosition;
    float nDotViewHalfVector = dot(normal, halfVector);
    float powerFactor = max(0.0, pow(nDotViewHalfVector, shininess));
    specular = lightSpecular * powerFactor;

    return ambient + diffuse + specular;
}

void main() {
    vec4 lightQD = pointLight(
    myBufferVals.uCamera.xyz,
    myBufferVals.lightPosition.xyz,
    myBufferVals.lightAmbient,
    myBufferVals.lightDiffuse,
    myBufferVals.lightSpecular,
    vNormal,
    vposition
    );

    outColor = genBoardColor(objPos) * lightQD;
}
