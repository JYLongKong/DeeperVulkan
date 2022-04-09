#include "MatrixState3D.h"
#include <cmath>

float MatrixState3D::vulkanClipMatrix[16];
float MatrixState3D::currMatrix[16];
float MatrixState3D::mProjMatrix[16];
float MatrixState3D::mVMatrix[16];
float MatrixState3D::mMVPMatrix[16];
float MatrixState3D::mStack[10][16];
int MatrixState3D::stackTop = -1;
float MatrixState3D::cx, MatrixState3D::cy, MatrixState3D::cz;

/**
 * 初始化基本变换矩阵
 */
void MatrixState3D::setInitStack() {
  Matrix::setIdentityM(currMatrix, 0);  // 将基本变换矩阵设置为单位矩阵

  // 初始化从OpenGL标准设备空间到Vulkan设备空间变换的矩阵
  // OpenGL标准设备空间XYZ三个轴范围都是从-1.0～+1.0
  // Vulkan设备空间XYZ三个轴范围分别是 -1.0～+1.0、+1.0～-1.0、0.0～+1.0
  // 变换时本质上采用的是缩放加平移矩阵
  // X轴不变Y轴置反Z轴缩放0.5
  // 缩放后Z轴正向平移0.5
  vulkanClipMatrix[0] = 1.0f;
  vulkanClipMatrix[1] = 0.0f;
  vulkanClipMatrix[2] = 0.0f;
  vulkanClipMatrix[3] = 0.0f;

  vulkanClipMatrix[4] = 0.0f;
  vulkanClipMatrix[5] = -1.0f;
  vulkanClipMatrix[6] = 0.0f;
  vulkanClipMatrix[7] = 0.0f;

  vulkanClipMatrix[8] = 0.0f;
  vulkanClipMatrix[9] = 0.0f;
  vulkanClipMatrix[10] = 0.5f;
  vulkanClipMatrix[11] = 0.0f;

  vulkanClipMatrix[12] = 0.0f;
  vulkanClipMatrix[13] = 0.0f;
  vulkanClipMatrix[14] = 0.5f;
  vulkanClipMatrix[15] = 1.0f;
}

/**
 * 保存基本变换矩阵入栈(保护现场)
 */
void MatrixState3D::pushMatrix() {
  stackTop++;
  for (int i = 0; i < 16; i++) {
    mStack[stackTop][i] = currMatrix[i];
  }
}

/**
 * 从栈恢复基本变换矩阵(恢复现场)
 */
void MatrixState3D::popMatrix() {
  for (int i = 0; i < 16; i++) {
    currMatrix[i] = mStack[stackTop][i];
  }
  stackTop--;
}

/**
 * 执行平移变换
 */
void MatrixState3D::translate(float x, float y, float z) {
  Matrix::translateM(currMatrix, 0, x, y, z);
}

/**
 * 执行旋转变换
 */
void MatrixState3D::rotate(float angle, float x, float y, float z) {
  Matrix::rotateM(currMatrix, 0, angle, x, y, z);
}

/**
 * 执行缩放变换
 */
void MatrixState3D::scale(float x, float y, float z) {
  Matrix::scaleM(currMatrix, 0, x, y, z);
}

/**
 * 设置摄像机
 */
void MatrixState3D::setCamera(
    float cx, float cy, float cz,   // 摄像机位置
    float tx, float ty, float tz,   // 目标点坐标
    float upx, float upy, float upz // up向量
) {
  // 记录摄像机位置坐标
  MatrixState3D::cx = cx;
  MatrixState3D::cy = cy;
  MatrixState3D::cz = cz;

  // 调用Matrix类的setLookAtM方法生成摄像机矩阵
  Matrix::setLookAtM(mVMatrix, 0, cx, cy, cz, tx, ty, tz, upx, upy, upz);
}

/**
 * 设置透视投影参数
 */
void MatrixState3D::setProjectFrustum(
    float left,
    float right,
    float bottom,
    float top,
    float near,
    float far
) {
  Matrix::frustumM(mProjMatrix, 0, left, right, bottom, top, near, far);
}

/**
 * 设置正交投影参数
 */
void MatrixState3D::setProjectOrtho(
    float left,
    float right,
    float bottom,
    float top,
    float near,
    float far
) {
  Matrix::orthoM(mProjMatrix, 0, left, right, bottom, top, near, far);
}

/**
 * 获取总变换矩阵
 */
float *MatrixState3D::getFinalMatrix() {
  Matrix::multiplyMM(mMVPMatrix, 0, mVMatrix, 0, currMatrix, 0);          // 当前基本变换矩阵与摄像机矩阵相乘
  Matrix::multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);       // 进一步乘以投影矩阵
  Matrix::multiplyMM(mMVPMatrix, 0, vulkanClipMatrix, 0, mMVPMatrix, 0);  // 乘以标准设备空间调整矩阵
  return mMVPMatrix;
}

/**
 * 获取当前基本变换矩阵
 */
float *MatrixState3D::getMMatrix() {
  return currMatrix;
}
