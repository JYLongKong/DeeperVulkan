#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, set = 0, binding = 0) uniform bufferVals {
    vec4 colorA;// 输入的颜色A
    vec4 colorB;// 输入的颜色B
} myBufferVals;

layout (location = 0) in vec3 vposition;// 顶点着色器传入的顶点位置
layout (location = 0) out vec4 outColor;// 输出到渲染管线的片元颜色值

// 棋盘纹理着色器实现方法
vec4 genBoardColor(vec3 position) {
    const float R = 1.0;// 球的半径
    vec4 color;// 结果颜色
    float n = 8.0;// 球体外接立方体每个坐标轴方向切分的份数
    float span = 2.0 * R / n;// 每一份的尺寸
    int i = int((position.x + 1.0) / span);// 当前片元位置小方块的行数
    int j = int((position.y + 1.0) / span);// 当前片元位置小方块的层数
    int k = int((position.z + 1.0) / span);// 当前片元位置小方块的列数
    int whichColor = int(mod(float(i + j + k), 2.0));// 计算行数、层数、列数的和并对2取模
    if (whichColor == 1) { // 奇数时为颜色A
        color = myBufferVals.colorA;
    } else { // 偶数时为颜色B
        color = myBufferVals.colorB;
    }

    return color;// 返回结果颜色
}

void main() {
    outColor = genBoardColor(vposition);// 将计算除的颜色传递给渲染管线
}
