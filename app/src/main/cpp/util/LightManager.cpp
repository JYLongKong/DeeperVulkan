#include "LightManager.h"

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
