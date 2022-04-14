#include "MatrixState3D.h"
#include "DrawableObjectCommon.h"
#include "Cube.h"
#include "mylog.h"

//#define UNIT_SIZE 30  // 立方体边长
//DrawableObjectCommon *colorRect;  // 指向绘制对象(正方形面物体)的指针
//
//Cube::Cube(VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties) {
//  ColorRect::genVertexData(); // 生成正方形顶点数据
//  colorRect = new DrawableObjectCommon( // 创建绘制对象(正方形)
//      ColorRect::vdata, ColorRect::dataByteCount, ColorRect::vCount, device, memoryroperties);
//}
//
//void Cube::drawSelf(VkCommandBuffer cmd,
//                    VkPipelineLayout &pipelineLayout,
//                    VkPipeline &pipeline,
//                    VkDescriptorSet *desSetPointer) {
//  // 前
//  MatrixState3D::pushMatrix();
//  MatrixState3D::translate(0, 0, UNIT_SIZE);
//  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
//  MatrixState3D::popMatrix();
//
//  // 后
//  MatrixState3D::pushMatrix();
//  MatrixState3D::translate(0, 0, -UNIT_SIZE);
//  MatrixState3D::rotate(180, 0, 1, 0);
//  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
//  MatrixState3D::popMatrix();
//
//  // 左
//  MatrixState3D::pushMatrix();
//  MatrixState3D::translate(-UNIT_SIZE, 0, 0);
//  MatrixState3D::rotate(-90, 0, 1, 0);
//  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
//  MatrixState3D::popMatrix();
//
//  // 右
//  MatrixState3D::pushMatrix();
//  MatrixState3D::translate(UNIT_SIZE, 0, 0);
//  MatrixState3D::rotate(90, 0, 1, 0);
//  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
//  MatrixState3D::popMatrix();
//
//  // 上
//  MatrixState3D::pushMatrix();
//  MatrixState3D::translate(0, UNIT_SIZE, 0);
//  MatrixState3D::rotate(-90, 1, 0, 0);
//  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
//  MatrixState3D::popMatrix();
//
//  // 下
//  MatrixState3D::pushMatrix();
//  MatrixState3D::translate(0, -UNIT_SIZE, 0);
//  MatrixState3D::rotate(90, 1, 0, 0);
//  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
//  MatrixState3D::popMatrix();
//}

/**
 * Sample4_12
 */
Cube::Cube(VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties, float *vdata, float unit_sizeIn) {
  unit_size = unit_sizeIn;
//  colorRect = new DrawableObjectCommon(vdata, ColorRect::dataByteCount, ColorRect::vCount, device, memoryroperties); // 为Sample4_16隐藏
}

/**
 * Sample4_12
 */
void Cube::drawSelf(VkCommandBuffer cmd,
                    VkPipelineLayout &pipelineLayout,
                    VkPipeline &pipeline,
                    VkDescriptorSet *desSetPointer) {
  // 绘制前小面
  MatrixState3D::pushMatrix();
  MatrixState3D::translate(0, 0, unit_size);
  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
  MatrixState3D::popMatrix();

  // 绘制后小面
  MatrixState3D::pushMatrix();
  MatrixState3D::translate(0, 0, -unit_size);
  MatrixState3D::rotate(180, 0, 1, 0);
  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
  MatrixState3D::popMatrix();

  // 绘制上大面
  MatrixState3D::pushMatrix();
  MatrixState3D::translate(0, unit_size, 0);
  MatrixState3D::rotate(-90, 1, 0, 0);
  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
  MatrixState3D::popMatrix();

  // 绘制下大面
  MatrixState3D::pushMatrix();
  MatrixState3D::translate(0, -unit_size, 0);
  MatrixState3D::rotate(90, 1, 0, 0);
  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
  MatrixState3D::popMatrix();

  // 绘制左大面
  MatrixState3D::pushMatrix();
  MatrixState3D::translate(unit_size, 0, 0);
  MatrixState3D::rotate(-90, 1, 0, 0);
  MatrixState3D::rotate(90, 0, 1, 0);
  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
  MatrixState3D::popMatrix();

  // 绘制右大面
  MatrixState3D::pushMatrix();
  MatrixState3D::translate(-unit_size, 0, 0);
  MatrixState3D::rotate(90, 1, 0, 0);
  MatrixState3D::rotate(-90, 0, 1, 0);
  colorRect->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
  MatrixState3D::popMatrix();
}

Cube::~Cube() {
  delete colorRect;
}
