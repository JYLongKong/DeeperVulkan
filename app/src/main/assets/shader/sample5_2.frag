#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 inLightQD;// 顶点着色器传入的光照强度
layout (location = 1) in vec3 vposition;// 顶点着色器传入的顶点位置
layout (location = 0) out vec4 outColor;// 输出到渲染管线的最终片元颜色值

vec4 genBoardColor(vec3 position) {
    const float R = 1.0;//球的半径
    vec4 color;
    float n = 8.0;// 外接立方体每个坐标轴方向切分的份数
    float span = 2.0 * R / n;// 每一份的尺寸(小方块的边长)
    // 为了保证不出现负数
    int i = int((position.x + 1.0) / span);// 当前片元位置小方块的行数
    int j = int((position.y + 1.0) / span);// 当前片元位置小方块的层数
    int k = int((position.z + 1.0) / span);// 当前片元位置小方块的列数
    // 计算当前片元行数、层数、列数的和并对2取模
    int whichColor = int(mod(float(i + j + k), 2.0));
    if (whichColor == 1) { // 奇数时为黑色
        color = vec4(0.0, 0.0, 0.0, 1.0);// 黑色
    } else { // 偶数时为白色
        color = vec4(1.0, 1.0, 1.0, 1.0);// 白色
    }

    return color;
}

void main() {
    outColor = inLightQD * genBoardColor(vposition);// 计算最终片元颜色
}
