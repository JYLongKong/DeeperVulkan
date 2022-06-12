#include "LightManager.h"
#include <math.h>

float LightManager::lx = 0;
float LightManager::ly = 0;
float LightManager::lz = 0;

float LightManager::ldx = 0;
float LightManager::ldy = 0;
float LightManager::ldz = 0;

float LightManager::lightAmbientR = 0;
float LightManager::lightAmbientG = 0;
float LightManager::lightAmbientB = 0;
float LightManager::lightAmbientA = 0;

float LightManager::lightDiffuseR = 0;
float LightManager::lightDiffuseG = 0;
float LightManager::lightDiffuseB = 0;
float LightManager::lightDiffuseA = 0;

float LightManager::lightSpecularR = 0;
float LightManager::lightSpecularG = 0;
float LightManager::lightSpecularB = 0;
float LightManager::lightSpecularA = 0;

float LightManager::lightFWJ;

void LightManager::setLightPosition(float lxIn, float lyIn, float lzIn) {
  lx = lxIn;
  ly = lyIn;
  lz = lzIn;
}

void LightManager::setLightDirection(float ldxIn, float ldyIn, float ldzIn) {
  ldx = ldxIn;
  ldy = ldyIn;
  ldz = ldzIn;
}

void LightManager::setLightAmbient(
    float lightAmbientRIn,
    float lightAmbientGIn,
    float lightAmbientBIn,
    float lightAmbientAIn
) {
  lightAmbientR = lightAmbientRIn;
  lightAmbientG = lightAmbientGIn;
  lightAmbientB = lightAmbientBIn;
  lightAmbientA = lightAmbientAIn;
}

void LightManager::setLightDiffuse(
    float lightDiffuseRIn,
    float lightDiffuseGIn,
    float lightDiffuseBIn,
    float lightDiffuseAIn
) {
  lightDiffuseR = lightDiffuseRIn;
  lightDiffuseG = lightDiffuseGIn;
  lightDiffuseB = lightDiffuseBIn;
  lightDiffuseA = lightDiffuseAIn;
}

void LightManager::setLightSpecular(
    float lightSpecularRIn,
    float lightSpecularGIn,
    float lightSpecularBIn,
    float lightSpecularAIn
) {
  lightSpecularR = lightSpecularRIn;
  lightSpecularG = lightSpecularGIn;
  lightSpecularB = lightSpecularBIn;
  lightSpecularA = lightSpecularAIn;
}

void LightManager::move(float fwjSpan) {
  lightFWJ = lightFWJ + fwjSpan; // 光源方位角
  float tempLx = float(sin(lightFWJ / 180 * 3.14150265) * 300); // 计算光源坐标X分量
  float tempLz = float(cos(lightFWJ / 180 * 3.14150265) * 300); // 计算光源坐标Z分量
  LightManager::setLightPosition(tempLx, 0, tempLz); // 设置光源位置
  if (lightFWJ >= 360) {
    lightFWJ = 0;
  }
}
