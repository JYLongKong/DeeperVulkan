#ifndef DEEPERVULKAN_LIGHTMANAGER_H
#define DEEPERVULKAN_LIGHTMANAGER_H

class LightManager {
 public:
  static float lightAmbientR, lightAmbientG, lightAmbientB, lightAmbientA;  // 环境光强度RGBA分量

  /**
   * 设置环境光强度
   */
  static void setLightAmbient(
      float lightAmbientRIn,
      float lightAmbientGIn,
      float lightAmbientBIn,
      float lightAmbientAIn
  );
};

#endif // DEEPERVULKAN_LIGHTMANAGER_H
