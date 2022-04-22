#ifndef DEEPERVULKAN_LIGHTMANAGER_H
#define DEEPERVULKAN_LIGHTMANAGER_H

class LightManager {
 public:
  static float lx, ly, lz;  // 光源位置
  static float lightAmbientR, lightAmbientG, lightAmbientB, lightAmbientA;  // 环境光强度RGBA分量
  static float lightDiffuseR, lightDiffuseG, lightDiffuseB, lightDiffuseA;  // 散射光强度RGBA分量
  static float lightSpecularR, lightSpecularG, lightSpecularB, lightSpecularA;  // 镜面光强度RGBA分量

  /**
   * 设置光源位置
   */
  static void setLightPosition(float lxIn, float lyIn, float lzIn);

  /**
   * 设置环境光强度
   */
  static void setLightAmbient(
      float lightAmbientRIn,
      float lightAmbientGIn,
      float lightAmbientBIn,
      float lightAmbientAIn
  );

  /**
   * 设置散射光强度
   */
  static void setLightDiffuse(
      float lightDiffuseRIn,
      float lightDiffuseGIn,
      float lightDiffuseBIn,
      float lightDiffuseAIn
  );

  /**
   * 设置镜面光强度
   */
  static void setLightSpecular(
      float lightSpecularRIn,
      float lightSpecularGIn,
      float lightSpecularBIn,
      float lightSpecularAIn
  );
};

#endif // DEEPERVULKAN_LIGHTMANAGER_H
