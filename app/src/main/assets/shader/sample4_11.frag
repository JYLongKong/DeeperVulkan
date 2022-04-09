#version 400

#extension GL_ARB_separate_shader_objects:enable
#extension GL_ARB_shading_language_420pack:enable
layout (location = 0) in vec3 vcolor;// 顶点着色器传入的顶点颜色数据
layout (location = 1) in vec3 vPosition;// 顶点着色器传入的顶点位置数据
layout (location = 0) out vec4 outColor;// 输出到渲染管线的片元颜色值

void main() {
    vec4 finalColor = vec4(vcolor.rgb, 0.0);// 用于计算片元颜色值的辅助变量
    mat4 mm = mat4(// 此旋转矩阵表示的是绕z轴旋转20°
    0.9396926, -0.34202012, 0.0, 0.0,
    0.34202012, 0.9396926, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0);
    vec4 tPosition = mm * vec4(vPosition, 1);// 将顶点坐标绕z轴转20°
    if (mod(tPosition.x + 50.0, 8) > 6) { // 计算x坐标是否在红色条带范围内
        finalColor = vec4(0.4, 0.0, 0.0, 1.0) + finalColor;// 若在则给最终颜色加上淡红色
    }
    outColor = finalColor;// 输出最终颜色值到渲染管线
}
