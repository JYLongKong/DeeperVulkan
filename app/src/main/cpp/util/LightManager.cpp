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
