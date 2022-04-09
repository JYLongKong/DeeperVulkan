#version 400                                                // 着色器版本号

#extension GL_ARB_separate_shader_objects : enable          // 启动GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable         // 启动GL_ARB_shading_language_420pack

/// Sample4_2
//layout (push_constant) uniform constantVals { // 推送常量块
//    mat4 mvp;// 最终变换矩阵
//} myConstantVals;

layout (std140, set = 0, binding = 0) uniform bufferVals { // 指定此一致块的绑定点编号为0
    mat4 mvp;// 包含用于接收总变换矩阵数据的成员mvp
} myBufferVals;// 声明一致块

layout (location = 0) in vec3 pos;// 传入的物体坐标系顶点坐标
layout (location = 1) in vec3 color;// 传入的顶点颜色
layout (location = 0) out vec3 vcolor;// 传到片元着色器的顶点颜色
//layout (location = 1) out vec3 vPosition;// Sample4_11

out gl_PerVertex { // 定义输出的接口块
    vec4 gl_Position;// 内建输出变量：负责接收最终的顶点位置并传递到渲染管线进行后继处理
//    float gl_PointSize;// Sample4_7-点的大小
};

void main() { // 主函数
    //    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);// Sample4_2

    gl_Position = myBufferVals.mvp * vec4(pos, 1.0);// 计算最终顶点位置
    //    gl_PointSize = 15.0;// Sample4_7-设置点的尺寸

    //    vPosition = gl_Position.xyz;// Sample4_11

    vcolor = color;// 传递顶点颜色给片元着色器
}
