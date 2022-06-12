#include "CameraUtil.h"
#include <math.h>
#include "MatrixState3D.h"

float CameraUtil::degree = 0;
float CameraUtil::yj = 0;
float CameraUtil::camera9Para[9];

void CameraUtil::calCamera(float yjSpan, float cxSpan) {
  yj = yj + yjSpan; // 更新仰角
  if (yj > 90) { yj = 90; } // 若仰角大于 90 度按照 90 度计算
  if (yj < -90) { yj = -90; } // 若仰角小于-90 度按照-90 度计算
  degree = degree + cxSpan; // 更新方位角
  if (degree > 360) { // 若方位角大于 360 度
    degree = degree - 360; // 将方位角减去 360
  } else if (degree < 0) { // 若方位角小于 0 度
    degree = degree + 360; // 将方位角加上 360
  }
  float cy = float(sin(yj * 3.1415926535898 / 180) * CAMERA_R); // 计算摄像机位置坐标 Y 分量
  float cxz = float(cos(yj * 3.1415926535898 / 180) * CAMERA_R);
  float cx = float(sin(degree * 3.1415926535898 / 180) * cxz); // 计算摄像机位置坐标 X 分量
  float cz = float(cos(degree * 3.1415926535898 / 180) * cxz); // 计算摄像机位置坐标 Z 分量
  float upY = float(cos(yj * 3.1415926535898 / 180)); // 计算 up 向量 Y 分量
  float upXZ = float(sin(yj * 3.1415926535898 / 180));
  float upX = float(-upXZ * sin(degree * 3.1415926535898 / 180)); // 计算 up 向量 X 分量
  float upZ = float(-upXZ * cos(degree * 3.1415926535898 / 180)); // 计算 up 向量 Z 分量
  camera9Para[0] = cx; // 记录摄像机坐标 X 分量
  camera9Para[1] = cy; // 记录摄像机坐标 Y 分量
  camera9Para[2] = cz; // 记录摄像机坐标 Z 分量
  camera9Para[3] = 0; // 记录目标点坐标 X 分量
  camera9Para[4] = 0; // 记录目标点坐标 Y 分量
  camera9Para[5] = 0; // 记录目标点坐标 Z 分量
  camera9Para[6] = upX; // 记录 up 向量 X 分量
  camera9Para[7] = upY; // 记录 up 向量 Y 分量
  camera9Para[8] = upZ; // 记录 up 向量 Z 分量
}

void CameraUtil::flushCameraToMatrix() {
  MatrixState3D::setCamera( // 设置摄像机的方法
      camera9Para[0], camera9Para[1], camera9Para[2], // 摄像机位置坐标 XYZ
      camera9Para[3], camera9Para[4], camera9Para[5], // 目标点坐标 XYZ
      camera9Para[6], camera9Para[7], camera9Para[8] // up 向量 XYZ 分量
  );
}
