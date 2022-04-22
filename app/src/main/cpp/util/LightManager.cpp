#include "LightManager.h"

float LightManager::lightAmbientR = 0;
float LightManager::lightAmbientG = 0;
float LightManager::lightAmbientB = 0;
float LightManager::lightAmbientA = 0;

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

/// Sample5_3 ************************************************** start
//float LightManager::lx = 0;
//float LightManager::ly = 0;
//float LightManager::lz = 0;
//float LightManager::lightDiffuseR = 0;
//float LightManager::lightDiffuseG = 0;
//float LightManager::lightDiffuseB = 0;
//float LightManager::lightDiffuseA = 0;
//
//void LightManager::setLightPosition(float lxIn, float lyIn, float lzIn) {
//  lx = lxIn;
//  ly = lyIn;
//  lz = lzIn;
//}
//
//void LightManager::setLightDiffuse(
//    float lightDiffuseRIn,
//    float lightDiffuseGIn,
//    float lightDiffuseBIn,
//    float lightDiffuseAIn
//) {
//  lightDiffuseR = lightDiffuseRIn;
//  lightDiffuseG = lightDiffuseGIn;
//  lightDiffuseB = lightDiffuseBIn;
//  lightDiffuseA = lightDiffuseAIn;
//}
/// Sample5_3 **************************************************** end

/// Sample5_4 ************************************************** start
float LightManager::lx = 0;
float LightManager::ly = 0;
float LightManager::lz = 0;
float LightManager::lightSpecularR = 0;
float LightManager::lightSpecularG = 0;
float LightManager::lightSpecularB = 0;
float LightManager::lightSpecularA = 0;

void LightManager::setLightPosition(float lxIn, float lyIn, float lzIn) {
  lx = lxIn;
  ly = lyIn;
  lz = lzIn;
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
/// Sample5_4 **************************************************** end
