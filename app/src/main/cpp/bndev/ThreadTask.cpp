#include "ThreadTask.h"
#include "MyVulkanManager.h"
#include "ShaderQueueSuit_Common.h"

ThreadTask::ThreadTask() {}

ThreadTask::~ThreadTask() {}

void ThreadTask::doTask() {
  MyVulkanManager::init_vulkan_instance();          // 创建Vulkan实例
  MyVulkanManager::enumerate_vulkan_phy_devices();  // 获取物理设备列表
  MyVulkanManager::create_vulkan_devices();         // 创建逻辑设备
  MyVulkanManager::create_vulkan_CommandBuffer();   // 创建命令缓冲
  MyVulkanManager::init_queue();                    // 获取设备中支持图形工作的队列
  MyVulkanManager::create_vulkan_swapChain();       // 初始化交换链
  MyVulkanManager::create_vulkan_DepthBuffer();     // 创建深度缓冲
  MyVulkanManager::create_render_pass();            // 创建渲染通道
  MyVulkanManager::create_frame_buffer();           // 创建帧缓冲
  MyVulkanManager::createDrawableObject();          // 创建绘制用的物体
  MyVulkanManager::initPipeline();                  // 初始化渲染管线
  MyVulkanManager::createFence();                   // 创建栅栏
  MyVulkanManager::initPresentInfo();               // 初始化呈现信息
//  MyVulkanManager::initMatrix();                    // 初始化基本变换矩阵、摄像机矩阵、投影矩阵
  MyVulkanManager::initMatrixAndLight();            // Sample5_2-初始化基本变换矩阵、摄像机矩阵、投影矩阵、光照
  MyVulkanManager::drawObject();                    // 执行绘制
  MyVulkanManager::destroyFence();                  // 销毁栅栏
  MyVulkanManager::destroyPipeline();               // 销毁管线
  MyVulkanManager::destroyDrawableObject();         // 销毁绘制用物体
  MyVulkanManager::destroy_frame_buffer();          // 销毁帧缓冲
  MyVulkanManager::destroy_render_pass();           // 销毁渲染通道相关
  MyVulkanManager::destroy_vulkan_DepthBuffer();    // 销毁深度缓冲相关
  MyVulkanManager::destroy_vulkan_swapChain();      // 销毁交换链相关
  MyVulkanManager::destroy_vulkan_CommandBuffer();  // 销毁命令缓冲
  MyVulkanManager::destroy_vulkan_devices();        // 销毁逻辑设备
  MyVulkanManager::destroy_vulkan_instance();       // 销毁Vulkan实例
}
