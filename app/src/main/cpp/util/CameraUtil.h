#ifndef DEEPERVULKAN_CAMERAUTIL_H_
#define DEEPERVULKAN_CAMERAUTIL_H_

#define CAMERA_R 600

class CameraUtil {
 public:
  static float degree; // 方位角
  static float yj;  // 仰角
  static float camera9Para[9];

  /**
   * 更新摄像机9参数
   */
  static void calCamera(float yjSpan, float cxSpan);

  /**
   * 根据摄像机9参数更新摄像机矩阵
   */
  static void flushCameraToMatrix();
};

#endif // DEEPERVULKAN_CAMERAUTIL_H_
