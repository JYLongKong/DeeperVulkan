#ifndef MatrixState_hpp // 防止重复定义
#define MatrixState_hpp

#include "Matrix.h" // 导入需要的头文件

class MatrixState3D {
 public:
  static float currMatrix[16];        // 当前基本变换矩阵
  static float mProjMatrix[16];       // 投影矩阵
  static float mVMatrix[16];          // 摄像机观察矩阵
  static float mMVPMatrix[16];        // 总变换矩阵
  static float vulkanClipMatrix[16];  // Vulkan专用标准设备空间调整矩阵(剪裁空间矩阵 X不变 Y置反 Z减半)
  static float mStack[10][16];        // 保存基本变换矩阵的栈
  static int stackTop;                // 栈顶索引
  static float cx, cy, cz;            // 摄像机位置坐标

  static void setInitStack();                                 // 初始化基本变换矩阵的方法
  static void pushMatrix();                                   // 保存基本变换矩阵入栈(保护现场)的方法
  static void popMatrix();                                    // 从栈恢复基本变换矩阵(恢复现场)的方法
  static void translate(float x, float y, float z);           // 沿x、y、z轴平移
  static void rotate(float angle, float x, float y, float z); // 绕指定轴旋转
  static void scale(float x, float y, float z);               // 矩阵缩放
  static void setCamera(                                      // 设置摄像机
      float cx, float cy, float cz,                           // 摄像机位置坐标
      float tx, float ty, float tz,                           // 目标点坐标
      float upx, float upy, float upz                         // 摄像机up向量
  );
  static void setProjectFrustum(                              // 设置透视投影参数
      float left,
      float right,
      float bottom,
      float top,
      float near,
      float far
  );
  static void setProjectOrtho(
      float left,
      float right,
      float bottom,
      float top,
      float near,
      float far
  );
  static float *getFinalMatrix();                             // 获取总变换矩阵
  static float *getMMatrix();                                 // 获取当前基本变换矩阵
};

#endif