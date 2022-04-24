#include <vulkan/vulkan.h>
#include <thread>
#include <iostream>
#include <cassert>
#include <chrono>
#include <ctime>

#include "../util/FileUtil.h"
#include "../util/HelpFunction.h"
#include "../util/FPSUtil.h"
#include "MyVulkanManager.h"
#include "ThreadTask.h"
#include "TriangleData.h"
#include "SixPointedStar.h"
#include "MatrixState3D.h"
#include "CubeData.h"
#include "ObjectData.h"
#include "BeltData.h"
#include "CircleData.h"
#include "ColorRect.h"
#include "BallData.h"
#include "LightManager.h"

// 静态成员实现
android_app *MyVulkanManager::Android_application;
bool MyVulkanManager::loopDrawFlag = true;
std::vector<const char *>  MyVulkanManager::instanceExtensionNames;
VkInstance MyVulkanManager::instance;
uint32_t MyVulkanManager::gpuCount;
std::vector<VkPhysicalDevice> MyVulkanManager::gpus;
uint32_t MyVulkanManager::queueFamilyCount;
std::vector<VkQueueFamilyProperties> MyVulkanManager::queueFamilyprops;
uint32_t MyVulkanManager::queueGraphicsFamilyIndex;
VkQueue MyVulkanManager::queueGraphics;
uint32_t MyVulkanManager::queuePresentFamilyIndex;
std::vector<const char *> MyVulkanManager::deviceExtensionNames;
VkDevice MyVulkanManager::device;
VkCommandPool MyVulkanManager::cmdPool;
VkCommandBuffer MyVulkanManager::cmdBuffer;
VkCommandBufferBeginInfo MyVulkanManager::cmd_buf_info;
VkCommandBuffer  MyVulkanManager::cmd_bufs[1];
VkSubmitInfo MyVulkanManager::submit_info[1];
uint32_t MyVulkanManager::screenWidth;
uint32_t MyVulkanManager::screenHeight;
VkSurfaceKHR MyVulkanManager::surface;
std::vector<VkFormat> MyVulkanManager::formats;
VkSurfaceCapabilitiesKHR MyVulkanManager::surfCapabilities;
uint32_t MyVulkanManager::presentModeCount;
std::vector<VkPresentModeKHR> MyVulkanManager::presentModes;
VkExtent2D MyVulkanManager::swapchainExtent;
VkSwapchainKHR MyVulkanManager::swapChain;
uint32_t MyVulkanManager::swapchainImageCount;
std::vector<VkImage> MyVulkanManager::swapchainImages;
std::vector<VkImageView> MyVulkanManager::swapchainImageViews;
VkFormat MyVulkanManager::depthFormat;
VkFormatProperties MyVulkanManager::depthFormatProps;
VkImage MyVulkanManager::depthImage;
VkPhysicalDeviceMemoryProperties MyVulkanManager::memoryroperties;
VkDeviceMemory MyVulkanManager::memDepth;
VkImageView MyVulkanManager::depthImageView;
VkSemaphore MyVulkanManager::imageAcquiredSemaphore;
uint32_t MyVulkanManager::currentBuffer;
VkRenderPass MyVulkanManager::renderPass;
VkClearValue MyVulkanManager::clear_values[2];
VkRenderPassBeginInfo MyVulkanManager::rp_begin;
VkFence MyVulkanManager::taskFinishFence;
VkPresentInfoKHR MyVulkanManager::present;
VkFramebuffer *MyVulkanManager::framebuffers;
ShaderQueueSuit_Common *MyVulkanManager::sqsCL;
float MyVulkanManager::xAngle = 0;

/// Sample4_1 ************************************************** start
int MyVulkanManager::vpCenterX = 0;
int MyVulkanManager::vpCenterY = 0;
DrawableObjectCommon *MyVulkanManager::triForDraw;
/// Sample4_1 **************************************************** end

/// Sample4_2 ************************************************** start
float MyVulkanManager::yAngle = 0;
DrawableObjectCommon *MyVulkanManager::objForDraw;
/// Sample4_2 **************************************************** end

/// Sample4_7
int MyVulkanManager::topologyWay = 0;

/// Sample4_8
DrawableObjectCommon *MyVulkanManager::cirForDraw;

/// Sample4_11
Cube *MyVulkanManager::cubeForDraw;
int MyVulkanManager::ViewPara = 0;

/// Sample4_12
//Cube *MyVulkanManager::cube1ForDraw;
//Cube *MyVulkanManager::cube2ForDraw;
//int MyVulkanManager::ProjectPara = 0;
//float MyVulkanManager::yAngle = 90;
//float MyVulkanManager::zAngle = 20;

/// Sample4_13
int MyVulkanManager::depthOffsetFlag = 0;
DrawableObjectCommon *MyVulkanManager::colorRectG;
DrawableObjectCommon *MyVulkanManager::colorRectY;

/// Sample4_14
//float MyVulkanManager::yAngle = 0;

/// Sample5_1
DrawableObjectCommon *MyVulkanManager::ballForDraw;

/**
 * 创建Vulkan实例的方法
 */
void MyVulkanManager::init_vulkan_instance() {
  AAssetManager *aam = MyVulkanManager::Android_application->activity->assetManager; // 获取资源管理器指针
  FileUtil::setAAssetManager(aam);                                          // 将资源管理器传给文件I/O工具类，以便在后面加载着色器脚本字符串
  if (!vk::loadVulkan()) {                                                  // 加载Vulkan动态库
    LOGE("load Vulkan application interfaces failed!");
    return;
  }

  VkApplicationInfo app_info = {};                                          // 构建应用信息结构体实例
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;                      // 结构体的类型
  app_info.pNext = nullptr;                                                 // 自定义数据的指针
  app_info.pApplicationName = "HelloDeepVulkan";                            // 应用的名称
  app_info.applicationVersion = 1;                                          // 应用的版本号
  app_info.pEngineName = "DeepVulkanEngine";                                // 应用的引擎名称
  app_info.engineVersion = 1;                                               // 应用引擎的版本号
  app_info.apiVersion = VK_API_VERSION_1_0;                                 // 使用的Vulkan图形应用程序API版本
  VkInstanceCreateInfo inst_info = {};                                      // 构建实例创建信息结构体实例
  inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;                 // 结构体的类型
  inst_info.pNext = nullptr;                                                // 自定义数据的指针
  inst_info.flags = 0;                                                      // 供将来使用的标志
  inst_info.pApplicationInfo = &app_info;                                   // 绑定应用信息结构体
  instanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);          // 该扩展名称列表用于支持KHR表面
  instanceExtensionNames.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);  // 该扩展名称列表用于支持Android下的KHR表面
  inst_info.enabledExtensionCount = instanceExtensionNames.size();          // 扩展的数量
  inst_info.ppEnabledExtensionNames = instanceExtensionNames.data();        // 扩展名称列表数据
  inst_info.enabledLayerCount = 0;                                          // 启动的层数量
  inst_info.ppEnabledLayerNames = nullptr;                                  // 启动的层名称列表

  VkResult result;                                                          // 存储运行结果的辅助变量
  result = vk::vkCreateInstance(&inst_info, nullptr, &instance);            // 创建Vulkan实例
  if (result == VK_SUCCESS) {                                               // 检查实例是否创建成功
    LOGI("Vulkan instance create successfully!");
  } else {
    LOGE("Vulkan instance create failed!");
  }
}

/**
 * 销毁Vulkan实例
 */
void MyVulkanManager::destroy_vulkan_instance() {
  vk::vkDestroyInstance(instance, nullptr);
  LOGI("destroy_vulkan_instance completed!");
}

/**
 * 获取物理设备列表
 */
void MyVulkanManager::enumerate_vulkan_phy_devices() {
  gpuCount = 0;                                                             // 存储物理设备数量的变量
  VkResult result = vk::vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr); // 获取物理设备数量
  assert(result == VK_SUCCESS);
  LOGI("Vulkan gpuCount = %d", gpuCount);
  gpus.resize(gpuCount);                                                    // 设置物理设备列表尺寸
  result = vk::vkEnumeratePhysicalDevices(instance, &gpuCount, gpus.data());// 填充物理设备列表
  assert(result == VK_SUCCESS);
  vk::vkGetPhysicalDeviceMemoryProperties(gpus[0], &memoryroperties);       // 获取第一物理设备的内存属性

  /// 获取不同平台下推送常量允许使用的最大总字节数
  VkPhysicalDeviceProperties gpuProps;                                      // 声明所需的结构体实例
  vk::vkGetPhysicalDeviceProperties(gpus[0], &gpuProps);                    // 获取各项属性值
  LOGI("VkPhysicalDeviceProperties::limits::maxPushConstantsSize=%d",
       gpuProps.limits.maxPushConstantsSize); // 最大推送常量总字节数
}

/**
 * 创建逻辑设备
 */
void MyVulkanManager::create_vulkan_devices() {
  vk::vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queueFamilyCount, nullptr); // 获取物理设备0中队列家族的数量
  LOGI("gpus[0].queueFamilyCount = %d", queueFamilyCount);
  queueFamilyprops.resize(queueFamilyCount);                                // 随队列家族数量改变vector长度
  vk::vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queueFamilyCount, queueFamilyprops.data()); // 填充物理设备0队列家族属性列表
  LOGI("successfully get queue of family properties from gpu[0]");

  VkDeviceQueueCreateInfo queueInfo = {};                                   // 构建设备队列创建信息结构体实例
  bool found = false;
  // 遍历所有队列家族属性列表，找到其中支持图形工作的一个队列家族并记录其索引
  for (unsigned int i = 0; i < queueFamilyCount; ++i) {
    if (queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {           // 若当前队列家族支持图形工作
      queueInfo.queueFamilyIndex = i;                                       // 绑定此队列家族索引
      queueGraphicsFamilyIndex = i;                                         // 记录支持图形工作的队列家族索引
      LOGI("the queue family index of supporting GRAPHICS work is %d", i);
      LOGI("the actual queue count of this family is %d", queueFamilyprops[i].queueCount);
      found = true;
      break;
    }
  }

  float queue_priorities[1] = {0.0};                                        // 创建队列优先级数组
  queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;             // 给出结构体类型
  queueInfo.pNext = nullptr;                                                // 自定义数据的指针
  queueInfo.queueCount = 1;                                                 // 指定队列数量
  queueInfo.pQueuePriorities = queue_priorities;                            // 给出每个队列的优先级
  queueInfo.queueFamilyIndex = queueGraphicsFamilyIndex;                    // 绑定队列家族索引
  deviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);          // 设置逻辑设备所需的扩展名称列表，使创建的设备支持交换链的使用

  /// Sample4_7 ************************************************** start
  VkPhysicalDeviceFeatures pdf;                                             // 存储设备所支持特性的结构体实例
  vk::vkGetPhysicalDeviceFeatures(gpus[0], &pdf);                           // 获取指定设备支持的特性
  if (pdf.wideLines == VK_TRUE) {                                           // 判断是否支持wideLines特性
    LOGI("support wideLines feature!");
  } else {
    LOGE("don't support wideLines feature!");
  }
  /// Sample4_7 **************************************************** end

  VkDeviceCreateInfo deviceInfo = {};                                       // 构建逻辑设备创建信息结构体实例
  deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;                  // 给出结构体类型
  deviceInfo.pNext = nullptr;                                               // 自定义数据的指针
  deviceInfo.queueCreateInfoCount = 1;                                      // 指定设备队列创建信息结构体数量
  deviceInfo.pQueueCreateInfos = &queueInfo;                                // 给定设备队列创建信息结构体列表
  deviceInfo.enabledExtensionCount = deviceExtensionNames.size();           // 所需扩展数量
  deviceInfo.ppEnabledExtensionNames = deviceExtensionNames.data();         // 所需扩展列表
  deviceInfo.enabledLayerCount = 0;                                         // 需启动Layer的数量
  deviceInfo.ppEnabledLayerNames = nullptr;                                 // 需启动Layer的名称列表
//  deviceInfo.pEnabledFeatures = nullptr;                                    // 启动的设备特性
  /// Sample4_7-创建的逻辑设备启用所有设备能够支持的特性(绘制宽度大于1的线需要启用wideLines特性)
  deviceInfo.pEnabledFeatures = &pdf;

  VkResult result = vk::vkCreateDevice(gpus[0], &deviceInfo, nullptr, &device); // 创建逻辑设备
  assert(result == VK_SUCCESS);                                             // 检查逻辑设备是否创建成功
}

/**
 * 销毁逻辑设备
 */
void MyVulkanManager::destroy_vulkan_devices() {
  vk::vkDestroyDevice(device, nullptr);
  LOGI("destroy_vulkan_devices completed！");
}

/**
 * 创建命令缓冲
 */
void MyVulkanManager::create_vulkan_CommandBuffer() {
  VkCommandPoolCreateInfo cmd_pool_info = {};                               // 构建命令池创建信息结构体实例
  cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;         // 给定结构体类型
  cmd_pool_info.pNext = nullptr;                                            // 自定义数据的指针
  cmd_pool_info.queueFamilyIndex = queueGraphicsFamilyIndex;                // 绑定所需队列家族索引
  cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;    // 执行控制标志
  VkResult result = vk::vkCreateCommandPool(device, &cmd_pool_info, nullptr, &cmdPool); // 创建命令池
  assert(result == VK_SUCCESS);                                             // 检查命令池创建是否成功

  VkCommandBufferAllocateInfo cmdBAI = {};                                  // 构建命令缓冲分配信息结构体实例
  cmdBAI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;            // 给定结构体类型
  cmdBAI.pNext = nullptr;                                                   // 自定义数据的指针
  cmdBAI.commandPool = cmdPool;                                             // 指定命令池
  cmdBAI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;                           // 分配的命令缓冲级别(主命令缓冲)
  cmdBAI.commandBufferCount = 1;                                            // 分配的命令缓冲数量
  result = vk::vkAllocateCommandBuffers(device, &cmdBAI, &cmdBuffer);       // 分配命令缓冲
  assert(result == VK_SUCCESS);                                             // 检查分配是否成功

  cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;         // 给定结构体类型
  cmd_buf_info.pNext = nullptr;                                             // 自定义数据的指针
  cmd_buf_info.flags = 0;                                                   // 描述使用标志(无特定使用情况)
  cmd_buf_info.pInheritanceInfo = nullptr;                                  // 命令缓冲继承信息(只有子命令缓冲分配时需要提供继承信息)
  cmd_bufs[0] = cmdBuffer;                                                  // 填充了提交到队列执行的命令缓冲数组

  auto *pipe_stage_flags = new VkPipelineStageFlags();                      // 目标管线阶段
  *pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;        // 设置等待的目标管线阶段为颜色附件输出
  submit_info[0].pNext = nullptr;                                           // 自定义数据的指针
  submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;                     // 给定结构体类型
  submit_info[0].pWaitDstStageMask = pipe_stage_flags;                      // 设置等待的目标管线阶段
  submit_info[0].commandBufferCount = 1;                                    // 命令缓冲数量
  submit_info[0].pCommandBuffers = cmd_bufs;                                // 提交的命令缓冲数组
  submit_info[0].signalSemaphoreCount = 0;                                  // 任务完毕后设置的信号量数量
  submit_info[0].pSignalSemaphores = nullptr;                               // 任务完毕后设置的信号量数组
}

/**
 * 销毁命令缓冲
 */
void MyVulkanManager::destroy_vulkan_CommandBuffer() {
  VkCommandBuffer cmdBufferArray[1] = {cmdBuffer};                          // 创建要释放的命令缓冲数组
  vk::vkFreeCommandBuffers(                                                 // 释放命令缓冲
      device,                                                               // 所属逻辑设备
      cmdPool,                                                              // 所属命令池
      1,                                                                    // 要销毁的命令缓冲数量
      cmdBufferArray                                                        // 要销毁的命令缓冲数组
  );
  vk::vkDestroyCommandPool(device, cmdPool, nullptr);                       // 销毁命令池
}

/**
 * 创建绘制用交换链
 */
void MyVulkanManager::create_vulkan_swapChain() {
  screenWidth = ANativeWindow_getWidth(Android_application->window);        // 获取屏幕宽度
  screenHeight = ANativeWindow_getHeight(Android_application->window);      // 获取屏幕高度
  LOGI("screen width: %d screen height: %d", screenWidth, screenHeight);

  VkAndroidSurfaceCreateInfoKHR createInfo;                                 // 构建KHR表面创建信息结构体实例
  createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;     // 给定结构体类型
  createInfo.pNext = nullptr;                                               // 自定义数据的指针
  createInfo.flags = 0;                                                     // 供未来使用的标志
  createInfo.window = Android_application->window;                          // 给定窗体
  auto fpCreateAndroidSurfaceKHR =                                          // 动态加载创建KHR表面的方法
      (PFN_vkCreateAndroidSurfaceKHR) vk::vkGetInstanceProcAddr(instance,
                                                                "vkCreateAndroidSurfaceKHR"); // 加载Android平台所需方法
  if (fpCreateAndroidSurfaceKHR == nullptr) {
    LOGE("can't find extension function: vkCreateAndroidSurfaceKHR");
  }
  VkResult result = fpCreateAndroidSurfaceKHR(instance, &createInfo, nullptr, &surface); // 创建Android平台用KHR表面
  assert(result == VK_SUCCESS);

  auto *pSupportsPresent = (VkBool32 *) malloc(queueFamilyCount * sizeof(VkBool32));
  for (uint32_t i = 0; i < queueFamilyCount; ++i) {                         // 遍历设备对应的队列家族列表
    vk::vkGetPhysicalDeviceSurfaceSupportKHR(gpus[0], i, surface, &pSupportsPresent[i]);
    LOGI("queue family Index = %d %s Present", i, (pSupportsPresent[i] == 1 ? "support" : "NOT support"));
  }

  queueGraphicsFamilyIndex = UINT32_MAX;                                    // 支持图形工作的队列家族索引
  queuePresentFamilyIndex = UINT32_MAX;                                     // 支持显示(呈现)工作的队列家族索引
  for (uint32_t i = 0; i < queueFamilyCount; ++i) {                         // 遍历设备对应的队列家族列表
    if ((queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {    // 若此队列家族支持图形工作
      if (queueGraphicsFamilyIndex == UINT32_MAX) {
        queueGraphicsFamilyIndex = i;                                       // 记录支持图形工作的队列家族索引
      }
      if (pSupportsPresent[i] == VK_TRUE) {                                 // 如果当前队列家族支持显示工作
        queueGraphicsFamilyIndex = i;                                       // 记录此队列家族索引为支持图形工作的
        queuePresentFamilyIndex = i;                                        // 记录此队列家族索引为支持显示工作的
        LOGI("queue family index = %d support both Graphics and Present work", i); // 找到既支持图形工作又支持显示工作的队列家族索引
        break;
      }
    }
  }
  if (queuePresentFamilyIndex == UINT32_MAX) {                              // 若没有找到同时支持两项工作的队列家族
    for (size_t i = 0; i < queueFamilyCount; ++i) {                         // 遍历设备对应的队列家族列表
      if (pSupportsPresent[i] == VK_TRUE) {                                 // 判断是否支持显示工作
        queuePresentFamilyIndex = i;                                        // 记录此队列家族索引为支持显示工作的
        break;
      }
    }
  }

  free(pSupportsPresent);                                                   // 释放存储是否支持呈现工作的布尔值列表

  if (queueGraphicsFamilyIndex == UINT32_MAX || queuePresentFamilyIndex == UINT32_MAX) {
    LOGE("NOT find queue family supports Graphics or Present work");        // 没有找到支持图形或显示工作的队列家族
    assert(false);                                                          // 若没有支持图形或显示操作的队列家族则程序终止
  }

  uint32_t formatCount;                                                     // 支持的格式数量
  result = vk::vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount, nullptr); // 获取KHR表面支持的格式数量
  LOGI("supported format count = %d", formatCount);

  auto *surfFormats = (VkSurfaceFormatKHR *) malloc(formatCount * sizeof(VkSurfaceFormatKHR)); // 分配对应数量的空间
  formats.resize(formatCount);                                              // 调整对应Vector尺寸
  result = vk::vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount, surfFormats); // 获取支持的格式信息列表
  for (int i = 0; i < formatCount; ++i) {                                   // 记录支持的格式信息列表
    formats[i] = surfFormats[i].format;
    LOGI("[%d] supported format is %d", i, formats[i]);
  }

  if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {   // 特殊情况处理
    formats[0] = VK_FORMAT_B8G8R8A8_UNORM;
  }
  free(surfFormats);                                                        // 释放辅助内存

  result = vk::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpus[0], surface, &surfCapabilities); // 获取KHR表面的能力
  assert(result == VK_SUCCESS);
  result = vk::vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[0], surface, &presentModeCount, nullptr); // 获取支持的显示模式数量
  assert(result == VK_SUCCESS);
  LOGI("presentModeCount = %d", presentModeCount);

  presentModes.resize(presentModeCount);                                    // 调整对应Vector尺寸
  result = vk::vkGetPhysicalDeviceSurfacePresentModesKHR(
      gpus[0], surface, &presentModeCount, presentModes.data());            // 获取指定数量显示模式的列表
  for (int i = 0; i < presentModeCount; ++i) {                              // 遍历打印所有显示模式的信息
    LOGI("present mode [%d] = %d", i, presentModes[i]);
  }

  VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;         // 确定交换链显示模式(FIFO模式是所有实现一般都支持)
  for (size_t i = 0; i < presentModeCount; ++i) {                           // 遍历显示模式列表
    if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {                   // 若支持MAILBOX模式
      // 如果也支持VK_PRESENT_MODE_MAILBOX_KHR模式，由于其效率高，便选用
      swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
      break;
    }
    if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {                 // 若支持IMMEDIATE模式
      // 如果没能用上VK_PRESENT_MODE_MAILBOX_KHR模式，但有VK_PRESENT_MODE_IMMEDIATE_KHR模式
      // 也比VK_PRESENT_MODE_FIFO_KHR模式强，故选用
      swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    }
  }

  // 确定交换链中图像的尺寸
  if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) {                 // 若表面没有确定尺寸
    swapchainExtent.width = screenWidth;                                    // 设置宽度为窗体宽度
    swapchainExtent.height = screenHeight;                                  // 设置高度为窗体高度
    if (swapchainExtent.width < surfCapabilities.minImageExtent.width) {    // 限制宽度在范围内
      swapchainExtent.width = surfCapabilities.minImageExtent.width;
    } else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width) {
      swapchainExtent.width = surfCapabilities.maxImageExtent.width;
    }
    if (swapchainExtent.height < surfCapabilities.minImageExtent.height) {  // 限制高度在范围内
      swapchainExtent.height = surfCapabilities.minImageExtent.height;
    } else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height) {
      swapchainExtent.height = surfCapabilities.maxImageExtent.height;
    }
    LOGI("use setting width * height = %d * %d", swapchainExtent.width, swapchainExtent.height);
  } else {
    swapchainExtent = surfCapabilities.currentExtent;                       // 若表面有确定尺寸
    LOGI("use surface width * height = %d * %d", swapchainExtent.width, swapchainExtent.height);
  }

  screenWidth = swapchainExtent.width;                                      // 记录实际采用的宽度
  screenHeight = swapchainExtent.height;                                    // 记录实际采用的高度
  uint32_t desiredMinNumberOfSwapChainImages = surfCapabilities.minImageCount + 1; // 期望交换链中的最少图像数量
  if ((surfCapabilities.maxImageCount > 0) && (desiredMinNumberOfSwapChainImages > surfCapabilities.maxImageCount)) {
    desiredMinNumberOfSwapChainImages = surfCapabilities.maxImageCount;     // 将图像数量限制到范围内
  }

  VkSurfaceTransformFlagBitsKHR preTransform;                               // KHR表面变换标志，变换指呈现时屏幕画面的旋转情况
  if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) { // 若支持(不旋转)变换
    preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  } else {                                                                  // 若不支持此变换类型
    preTransform = surfCapabilities.currentTransform;                       // 则设置标志值为KHR表面能力中的当前变换标志值
  }

  VkSwapchainCreateInfoKHR swapchain_ci = {};                               // 构建交换链创建信息结构体实例
  swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;         // 结构体类型
  swapchain_ci.pNext = nullptr;                                             // 自定义数据的指针
  swapchain_ci.surface = surface;                                           // 指定KHR表面
  swapchain_ci.minImageCount = desiredMinNumberOfSwapChainImages;           // 最少图像数量
  swapchain_ci.imageFormat = formats[0];                                    // 图像格式
  swapchain_ci.imageExtent.width = swapchainExtent.width;                   // 交换链图像宽度
  swapchain_ci.imageExtent.height = swapchainExtent.height;                 // 交换链图像高度
  swapchain_ci.preTransform = preTransform;                                 // 指定变换标志
  swapchain_ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;          // 混合Alpha值
  swapchain_ci.imageArrayLayers = 1;                                        // 图像数组层数
  swapchain_ci.presentMode = swapchainPresentMode;                          // 交换链的显示模式
  swapchain_ci.oldSwapchain = VK_NULL_HANDLE;                               // 前导交换链
  swapchain_ci.clipped = true;                                              // 开启裁剪
  swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;          // 色彩空间
  swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;            // 图像用途(表示将作为帧缓冲的颜色附件使用)
  swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;                // 图像共享模式
  swapchain_ci.queueFamilyIndexCount = 0;                                   // 队列家族数量
  swapchain_ci.pQueueFamilyIndices = nullptr;                               // 队列家族索引列表

  if (queueGraphicsFamilyIndex != queuePresentFamilyIndex) {                // 若支持图形和显示工作的队列家族不相同，则需要更改交换链的共享模式
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;             // 该共享模式旨在绘制过程中根据需要将图像在两个队列家族的队列之间传输
    swapchain_ci.queueFamilyIndexCount = 2;                                 // 交换链所需的队列家族索引数量为2
    uint32_t queueFamilyIndices[2] = {queueGraphicsFamilyIndex, queuePresentFamilyIndex};
    swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;                  // 交换链所需的队列家族索引列表
  }

  result = vk::vkCreateSwapchainKHR(device, &swapchain_ci, nullptr, &swapChain); // 创建交换链
  assert(result == VK_SUCCESS);                                             // 检查交换链是否创建成功

  result = vk::vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, nullptr); // 获取交换链中的图像数量
  assert(result == VK_SUCCESS);                                             // 检查是否获取成功
  LOGI("swapchainImageCount = %d", swapchainImageCount);
  swapchainImages.resize(swapchainImageCount);                              // 调整图像列表尺寸
  result = vk::vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, swapchainImages.data()); // 获取交换链中的图像列表
  assert(result == VK_SUCCESS);                                             // 检查是否获取成功
  swapchainImageViews.resize(swapchainImageCount);                          // 调整图像视图列表尺寸

  for (uint32_t i = 0; i < swapchainImageCount; ++i) {                      // 为交换链中的各幅图像创建图像视图
    VkImageViewCreateInfo color_image_view = {};                            // 构建图像视图创建信息结构体实例
    color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;      // 设置结构体类型
    color_image_view.pNext = nullptr;                                       // 自定义数据的指针
    color_image_view.flags = 0;                                             // 供将来使用的标志
    color_image_view.image = swapchainImages[i];                            // 对应交换链图像
    color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;                      // 图像视图的类型
    color_image_view.format = formats[0];                                   // 图像视图格式
    color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;                 // 设置R通道调和
    color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;                 // 设置G通道调和
    color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;                 // 设置B通道调和
    color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;                 // 设置A通道调和
    color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // 图像视图使用方面(每个像素用于存储颜色值)
    color_image_view.subresourceRange.baseMipLevel = 0;                     // 基础Mipmap级别
    color_image_view.subresourceRange.levelCount = 1;                       // Mipmap级别的数量
    color_image_view.subresourceRange.baseArrayLayer = 0;                   // 基础数组层
    color_image_view.subresourceRange.layerCount = 1;                       // 数组层的数量
    result = vk::vkCreateImageView(device, &color_image_view, nullptr, &swapchainImageViews[i]); // 创建图像视图
    assert(result == VK_SUCCESS);                                           // 检查是否创建成功
  }
}

/**
 * 销毁交换链相关
 */
void MyVulkanManager::destroy_vulkan_swapChain() {
  for (uint32_t i = 0; i < swapchainImageCount; i++) {
    vk::vkDestroyImageView(device, swapchainImageViews[i], nullptr);
    LOGI("Destroy swapchainImageViews[%d] success!", i);
  }
  vk::vkDestroySwapchainKHR(device, swapChain, nullptr);
  LOGI("Destroy SwapChain success!");
}

/**
 * 创建深度缓冲
 */
void MyVulkanManager::create_vulkan_DepthBuffer() {
  depthFormat = VK_FORMAT_D16_UNORM;                                      // 指定深度图像的格式
  VkImageCreateInfo image_info = {};                                      // 构建深度图像创建信息结构体实例
  vk::vkGetPhysicalDeviceFormatProperties(gpus[0], depthFormat, &depthFormatProps); // 获取物理设备支持的指定格式的属性

  // 通过物理设备支持的指定格式的属性确定深度图像的平铺方式
  if (depthFormatProps.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) { // 是否支持线性瓦片组织方式
    image_info.tiling = VK_IMAGE_TILING_LINEAR;                           // 采用线性瓦片组织方式
    LOGI("tiling = VK_IMAGE_TILING_LINEAR!");
  } else if (depthFormatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) { // 是否支持最优瓦片组织方式
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;                          // 采用最优瓦片组织方式
    LOGI("tiling = VK_IMAGE_TILING_OPTIMAL!");
  } else {
    LOGE("unsupported VK_FORMAT_D16_UNORM!");                             // 打印不支持指定格式的提示信息
  }

  image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;                 // 指定结构体类型
  image_info.pNext = nullptr;                                             // 自定义数据的指针
  image_info.imageType = VK_IMAGE_TYPE_2D;                                // 图像类型
  image_info.format = depthFormat;                                        // 图像格式
  image_info.extent.width = screenWidth;                                  // 图像宽度
  image_info.extent.height = screenHeight;                                // 图像高度
  image_info.extent.depth = 1;                                            // 图像深度
  image_info.mipLevels = 1;                                               // 图像Mipmap级数
  image_info.arrayLayers = 1;                                             // 图像数组层数量
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;                             // 采样模式
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;                   // 初始布局
  image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;         // 图像用途
  image_info.queueFamilyIndexCount = 0;                                   // 队列家族数量(共享模式为EXCLUSIVE时应该为0)
  image_info.pQueueFamilyIndices = nullptr;                               // 队列家族索引列表(共享模式为EXCLUSIVE时应设置为空)
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;                     // 共享模式(表示图像不允许被多个队列家族的队列访问，CONCURRENT则允许)
  image_info.flags = 0;                                                   // 标志

  VkMemoryAllocateInfo mem_alloc = {};                                    // 构建内存分配信息结构体实例
  mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;               // 结构体类型
  mem_alloc.pNext = nullptr;                                              // 自定义数据的指针
  mem_alloc.allocationSize = 0;                                           // 分配的内存字节数
  mem_alloc.memoryTypeIndex = 0;                                          // 内存的类型索引

  VkImageViewCreateInfo view_info = {};                                   // 构建深度图像视图创建信息结构体实例
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;             // 设置结构体类型
  view_info.pNext = nullptr;                                              // 自定义数据的指针
  view_info.image = VK_NULL_HANDLE;                                       // 对应的图像
  view_info.format = depthFormat;                                         // 图像视图的格式
  view_info.components.r = VK_COMPONENT_SWIZZLE_R;                        // 设置R通道调和
  view_info.components.g = VK_COMPONENT_SWIZZLE_G;                        // 设置G通道调和
  view_info.components.b = VK_COMPONENT_SWIZZLE_B;                        // 设置B通道调和
  view_info.components.a = VK_COMPONENT_SWIZZLE_A;                        // 设置A通道调和
  view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;      // 图像视图使用方面
  view_info.subresourceRange.baseMipLevel = 0;                            // 基础Mipmap级别
  view_info.subresourceRange.levelCount = 1;                              // Mipmap级别的数量
  view_info.subresourceRange.baseArrayLayer = 0;                          // 基础数组层
  view_info.subresourceRange.layerCount = 1;                              // 数组层的数量
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;                             // 图像视图的类型
  view_info.flags = 0;                                                    // 标志

  VkResult result = vk::vkCreateImage(device, &image_info, nullptr, &depthImage); // 创建深度图像(等待分配内存)
  assert(result == VK_SUCCESS);

  VkMemoryRequirements mem_reqs;
  vk::vkGetImageMemoryRequirements(device, depthImage, &mem_reqs);        // 获取图像内存需求
  mem_alloc.allocationSize = mem_reqs.size;                               // 获取所需内存字节数
  VkFlags requirements_mask = 0;                                          // 需要的内存类型掩码(0表示无特殊内存要求)
  bool flag = memoryTypeFromProperties(                                   // 获取所需内存类型索引
      memoryroperties, mem_reqs.memoryTypeBits, requirements_mask, &mem_alloc.memoryTypeIndex);
  assert(flag);                                                           // 检查获取是否成功
  LOGI("confirm memory type success, memoryTypeIndex = %d", mem_alloc.memoryTypeIndex);

  result = vk::vkAllocateMemory(device, &mem_alloc, nullptr, &memDepth);  // 分配内存
  assert(result == VK_SUCCESS);
  result = vk::vkBindImageMemory(device, depthImage, memDepth, 0);        // 绑定图像和内存，这才确定图像在设备内存中的存储位置
  assert(result == VK_SUCCESS);
  view_info.image = depthImage;                                           // 指定图像视图对应图像

  // Vulkan中的图像不能直接被访问，必须通过配套的图像视图进行访问
  result = vk::vkCreateImageView(device, &view_info, nullptr, &depthImageView); // 创建深度图像视图
  assert(result == VK_SUCCESS);
}

/**
 * 销毁深度缓冲相关
 */
void MyVulkanManager::destroy_vulkan_DepthBuffer() {
  vk::vkDestroyImageView(device, depthImageView, nullptr);
  vk::vkDestroyImage(device, depthImage, nullptr);
  vk::vkFreeMemory(device, memDepth, nullptr);
  LOGI("destroy_vulkan_DepthBuffer success!");
}

/**
 * 创建渲染通道
 */
void MyVulkanManager::create_render_pass() {
  VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;                 // 构建信号量创建信息结构体实例
  imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO; // 结构体类型
  imageAcquiredSemaphoreCreateInfo.pNext = nullptr;                       // 自定义数据的指针
  imageAcquiredSemaphoreCreateInfo.flags = 0;                             // 供将来使用的标志
  VkResult result = vk::vkCreateSemaphore(                                // 创建信号量
      device, &imageAcquiredSemaphoreCreateInfo, nullptr, &imageAcquiredSemaphore);
  assert(result == VK_SUCCESS);                                           // 检测信号量是否创建成功

  VkAttachmentDescription attachments[2];                                 // 声明了长度为2的附件描述信息数组
  // 颜色附件描述信息
  attachments[0].format = formats[0];                                     // 设置颜色附件的格式
  attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;                         // 设置采样模式
  attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                    // 子渲染通道开始时的操作(针对颜色附件)
  attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;                  // 子渲染通道结束时的操作(针对颜色附件)
  attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;         // 子渲染通道开始时的操作(针对模板附件)
  attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;       // 子渲染通道结束时的操作(针对模板附件)
  attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;               // 开始时的布局
  attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;           // 结束时的最终布局(将画面进行呈现)
  attachments[0].flags = 0;                                               // 设置位掩码
  // 深度附件描述信息
  attachments[1].format = depthFormat;                                    // 设置位深附件的格式
  attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;                         // 设置采样模式
  attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                    // 子渲染通道开始时的操作(针对深度附件)
  attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;              // 子渲染通道结束时的操作(针对深度附件)
  attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;         // 子渲染通道开始时的操作(针对模板附件)
  attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;       // 子渲染通道结束时的操作(针对模板附件)
  attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;               // 开始时的布局
  attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // 结束时的布局
  attachments[1].flags = 0;                                               // 设置位掩码

  VkAttachmentReference color_reference = {};                             // 颜色附件引用
  color_reference.attachment = 0;                                         // 对应附件描述信息数组下标
  color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;      // 设置附件布局

  VkAttachmentReference depth_reference = {};                             // 深度附件引用
  depth_reference.attachment = 1;                                         // 对应附件描述信息数组下标
  depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // 设置附件布局

  VkSubpassDescription subpass = {};                                      // 构建渲染子通道描述信息结构体实例
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;            // 设置管线绑定点
  subpass.flags = 0;                                                      // 设置掩码
  subpass.inputAttachmentCount = 0;                                       // 输入附件数量
  subpass.pInputAttachments = nullptr;                                    // 输入附件列表
  subpass.colorAttachmentCount = 1;                                       // 颜色附件数量
  subpass.pColorAttachments = &color_reference;                           // 颜色附件列表
  subpass.pResolveAttachments = nullptr;                                  // Resolve附件
  subpass.pDepthStencilAttachment = &depth_reference;                     // 深度模板附件
  subpass.preserveAttachmentCount = 0;                                    // preserve附件数量
  subpass.pPreserveAttachments = nullptr;                                 // preserve附件列表

  VkRenderPassCreateInfo rp_info = {};                                    // 构建渲染通道创建信息结构体实例
  rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;              // 结构体类型
  rp_info.pNext = nullptr;                                                // 自定义数据的指针
  rp_info.attachmentCount = 2;                                            // 附件的数量
  rp_info.pAttachments = attachments;                                     // 附件列表
  rp_info.subpassCount = 1;                                               // 渲染子通道数量
  rp_info.pSubpasses = &subpass;                                          // 渲染子通道列表
  rp_info.dependencyCount = 0;                                            // 子通道依赖数量
  rp_info.pDependencies = nullptr;                                        // 子通道依赖列表
  result = vk::vkCreateRenderPass(device, &rp_info, nullptr, &renderPass); // 创建渲染通道
  assert(result == VK_SUCCESS);                                           // 检查是否创建成功

  clear_values[0].color.float32[0] = 0.0f;                                // 帧缓冲清除用R分量值
  clear_values[0].color.float32[1] = 0.0f;                                // 帧缓冲清除用G分量值
  clear_values[0].color.float32[2] = 0.0f;                                // 帧缓冲清除用B分量值
  clear_values[0].color.float32[3] = 0.0f;                                // 帧缓冲清除用A分量值
  clear_values[1].depthStencil.depth = 1.0f;                              // 帧缓冲清除用深度值
  clear_values[1].depthStencil.stencil = 0;                               // 帧缓冲清除用模板值

  rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;              // 渲染通道启动信息结构体类型
  rp_begin.pNext = nullptr;                                               // 自定义数据的指针
  rp_begin.renderPass = renderPass;                                       // 指定要启动的渲染通道
  rp_begin.renderArea.offset.x = 0;                                       // 渲染区域起始x坐标
  rp_begin.renderArea.offset.y = 0;                                       // 渲染区域起始y坐标
  rp_begin.renderArea.extent.width = screenWidth;                         // 渲染区域宽度
  rp_begin.renderArea.extent.height = screenHeight;                       // 渲染区域高度
  rp_begin.clearValueCount = 2;                                           // 帧缓冲清除值数量
  rp_begin.pClearValues = clear_values;                                   // 帧缓冲清除值数组
}

/**
 * 销毁渲染通道相关
 */
void MyVulkanManager::destroy_render_pass() {
  vk::vkDestroyRenderPass(device, renderPass, nullptr);
  vk::vkDestroySemaphore(device, imageAcquiredSemaphore, nullptr);
}

/**
 * 获取设备中支持图形工作的队列
 */
void MyVulkanManager::init_queue() {
  vk::vkGetDeviceQueue(device, queueGraphicsFamilyIndex, 0, &queueGraphics); // 获取指定家族中索引为0的队列
}

/**
 * 创建帧缓冲
 */
void MyVulkanManager::create_frame_buffer() {
  VkImageView attachments[2];                                             // 附件图像视图数组
  attachments[1] = depthImageView;                                        // 给定深度图像视图

  VkFramebufferCreateInfo fb_info = {};                                   // 构建帧缓冲创建信息结构体实例
  fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;              // 结构体类型
  fb_info.pNext = nullptr;                                                // 自定义数据的指针
  fb_info.renderPass = renderPass;                                        // 指定渲染通道
  fb_info.attachmentCount = 2;                                            // 附件数量
  fb_info.pAttachments = attachments;                                     // 附件图像视图数组
  fb_info.width = screenWidth;                                            // 宽度
  fb_info.height = screenHeight;                                          // 高度
  fb_info.layers = 1;                                                     // 层数

  // 根据交换链中的图像数量分配了对应数量帧缓冲所需的内存
  framebuffers = (VkFramebuffer *) malloc(swapchainImageCount * sizeof(VkFramebuffer)); // 为帧缓冲序列动态分配内存
  assert(framebuffers);                                                   // 检查内存分配是否成功

  for (uint32_t i = 0; i < swapchainImageCount; ++i) {                    // 遍历交换链中的各个图像
    attachments[0] = swapchainImageViews[i];                              // 给定颜色附件对应图像视图
    VkResult result = vk::vkCreateFramebuffer(device, &fb_info, nullptr, &framebuffers[i]); // 为每一个图像创建对应帧缓冲
    assert(result == VK_SUCCESS);                                         // 检查是否创建成功
    LOGI("create framebuffer[%d] success!", i);
  }
}

/**
 * 销毁帧缓冲
 * 遍历销毁交换链中各个图像对应的帧缓冲
 */
void MyVulkanManager::destroy_frame_buffer() {
  for (int i = 0; i < swapchainImageCount; i++) {
    vk::vkDestroyFramebuffer(device, framebuffers[i], nullptr);
  }
  free(framebuffers);
  LOGI("destroy_frame_buffer success!");
}

/**
 * 创建绘制用物体
 */
void MyVulkanManager::createDrawableObject() {
  /// Sample4_1、Sample4_14 ************************************** start
//  TriangleData::genVertexData();                                        // 生成3色三角形顶点数据和颜色数据
//  triForDraw = new DrawableObjectCommon(                                // 创建绘制用三色三角形对象
//      TriangleData::vdata, TriangleData::dataByteCount, TriangleData::vCount, device, memoryroperties);
  /// Sample4_1、Sample4_14 **************************************** end

  /// Sample4_2 ************************************************** start
//  SixPointedStar::genStarData(0.2, 0.5, 0);
//  objForDraw = new DrawableObjectCommon(
//      SixPointedStar::vdata, SixPointedStar::dataByteCount, SixPointedStar::vCount, device, memoryroperties);
  /// Sample4_2 **************************************************** end

  /// Sample4_4 ************************************************** start
//  CubeData::genBallData();
//  objForDraw = new DrawableObjectCommon(
//      CubeData::vdata, CubeData::dataByteCount, CubeData::vCount, device, memoryroperties);
  /// Sample4_4 **************************************************** end

  /// Sample4_7 ************************************************** start
//  ObjectData::genVertexData();
//  objForDraw = new DrawableObjectCommon(
//      ObjectData::vdata, ObjectData::dataByteCount, ObjectData::vCount, device, memoryroperties);
  /// Sample4_7 **************************************************** end

  /// Sample4_8 ************************************************** start
//  BeltData::genVertexData();
//  triForDraw = new DrawableObjectCommon(
//      BeltData::vdata, BeltData::dataByteCount, BeltData::vCount, device, memoryroperties);
//  CircleData::genVertexData();
//  cirForDraw = new DrawableObjectCommon(
//      CircleData::vdata, CircleData::dataByteCount, CircleData::vCount, device, memoryroperties);
  /// Sample4_8 **************************************************** end

  /// Sample4_10、Sample4_16 ************************************** start
//  CircleData::genVertexData();
//  cirForDraw = new DrawableObjectCommon(
//      CircleData::vdata,
//      CircleData::dataByteCount,
//      CircleData::vCount,
//      CircleData::idata,
//      CircleData::indexByteCount,
//      CircleData::iCount,
//      device,
//      memoryroperties);
  /// Sample4_10、Sample4_16 *************************************** end

  /// Sample4_11 ************************************************* start
//  cubeForDraw = new Cube(device, memoryroperties);
  /// Sample4_11 *************************************************** end

  /// Sample4_12 ************************************************* start
//  ColorRect::genVertexData();
//  cube1ForDraw = new Cube(device, memoryroperties, ColorRect::vdataG, ColorRect::UNIT_SIZEG);
//  cube2ForDraw = new Cube(device, memoryroperties, ColorRect::vdataY, ColorRect::UNIT_SIZEY);
  /// Sample4_12 *************************************************** end

  /// Sample4_13 ************************************************* start
//  ColorRect::genVertexData();
//  colorRectG = new DrawableObjectCommon(
//      ColorRect::vdataG, ColorRect::dataByteCount, ColorRect::vCount, device, memoryroperties);
//  colorRectY = new DrawableObjectCommon(
//      ColorRect::vdataY, ColorRect::dataByteCount, ColorRect::vCount, device, memoryroperties);
  /// Sample4_13 *************************************************** end

  /// Sample5_1 ************************************************** start
  BallData::genBallData(9);                                      // 生成球面的顶点数据
  ballForDraw = new DrawableObjectCommon(                                 // 创建绘制用球对象
      BallData::vdata, BallData::dataByteCount, BallData::vCount, device, memoryroperties);
  /// Sample5_1 **************************************************** end
}

/**
 * 销毁绘制用物体
 */
void MyVulkanManager::destroyDrawableObject() {
  /// Sample4_1、Sample4_14
//  delete triForDraw;

  /// Sample4_2
//  delete objForDraw;

  /// Sample4_8
//  delete triForDraw;
//  delete cirForDraw;

  /// Sample4_11
//  delete cubeForDraw;

  /// Sample4_12
//  delete cube1ForDraw;
//  delete cube2ForDraw;

  /// Sample4_13
//  delete colorRectG;
//  delete colorRectY;

  /// Sample5_1
  delete ballForDraw;
}

/**
 * 创建用于等待指定任务执行完毕的栅栏
 */
void MyVulkanManager::createFence() {
  VkFenceCreateInfo fenceInfo;                                            // 栅栏创建信息结构体实例
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.pNext = nullptr;
  // 若设置为VK_FENCE_CREATE_SIGNALED_BIT，则栅栏的初始状态为触发态(一般代表任务已完成)
  fenceInfo.flags = 0;                                                    // 栅栏在初始状态下为未触发态(一般代表任务未完成)
  vk::vkCreateFence(device, &fenceInfo, nullptr, &taskFinishFence);       // 创建栅栏
}

/**
 * 销毁栅栏
 */
void MyVulkanManager::destroyFence() {
  vk::vkDestroyFence(device, taskFinishFence, nullptr);
}

/**
 * 初始化呈现信息
 */
void MyVulkanManager::initPresentInfo() {
  present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present.pNext = nullptr;
  present.swapchainCount = 1;                                             // 交换链的数量
  present.pSwapchains = &swapChain;                                       // 交换链列表
  present.waitSemaphoreCount = 0;                                         // 等待的信号量数量
  present.pWaitSemaphores = nullptr;                                      // 等待的信号量列表
  // 呈现操作结果标志列表：指向元素数量与交换链数量相同的VkResult型数组首元素的指针，
  // 当呈现完成后Vulkan会根据各个交换链执行呈现的情况来填充此数组对应的元素值
  present.pResults = nullptr;
}

/**
 * 初始化基本变换矩阵、摄像机矩阵和投影矩阵
 * Sample5_2-新增初始化环境光照
 */
//void MyVulkanManager::initMatrix() {
void MyVulkanManager::initMatrixAndLight() {
  // 初始化摄像机
//  MatrixState3D::setCamera(0, 0, 200, 0, 0, 0, 0, 1, 0); // Sample4_14、Sample4_16
//  MatrixState3D::setCamera(0, 0, 2, 0, 0, 0, 0, 1, 0); // Sample4_2
//  MatrixState3D::setCamera(-16, 8, 45, 0, 0, 0, 0, 1.0, 0.0); // Sample4_4
//  MatrixState3D::setCamera(0, 0, 200, 0, 0, 0, 0, 1, 0); // Sample4_7
//  MatrixState3D::setCamera(0, 0, 3, 0, 0, 0, 0, 1, 0);  // Sample5_1
  MatrixState3D::setCamera(0, 0, 30.0f, 0, 0, 0, 0, 1, 0);  // Sample5_2

  MatrixState3D::setInitStack();                                          // 初始化基本变换矩阵
  float ratio = (float) screenWidth / (float) screenHeight;               // 求屏幕宽高比

  // 设置投影参数
//  MatrixState3D::setProjectFrustum(-ratio, ratio, -1, 1, 1.5f, 1000); // Sample4_14、Sample4_16、Sample5_1
//  MatrixState3D::setProjectOrtho(-ratio, ratio, -1, 1, 1.0f, 20); // Sample4_2-设置正交投影参数
//  MatrixState3D::setProjectFrustum(-ratio * 0.4, ratio * 0.4, -1 * 0.4, 1 * 0.4, 1.0f, 20); // Sample4_3-设置透视投影参数
//  MatrixState3D::setProjectFrustum(-ratio * 0.8f, ratio * 1.2f, -1, 1, 20, 100); // Sample4_4
//  MatrixState3D::setProjectFrustum(-ratio, ratio, -1, 1, 1.5f, 1000); // Sample4_7
  MatrixState3D::setProjectFrustum(-ratio, ratio, -1, 1, 20.0f, 1000);  // Sample5_2

  /// Sample4_11 ************************************************* start
//  if (ViewPara) { // 合理的视角
//    MatrixState3D::setCamera(0, 50, 200, 0, 0, 0, 0, 1, 0); // 设置合理视角下的摄像机
//    MatrixState3D::setProjectFrustum(-ratio, ratio, -1, 1, 1.5f, 1000); // 合理视角下的投影参数
//  } else {  // 不合理的视角
//    MatrixState3D::setCamera(0, 50, 100, 0, 0, 0, 0, 1, 0); // 设置不合理视角下的摄像机
//    MatrixState3D::setProjectFrustum(-ratio * 0.7, ratio * 0.7, -0.7, 0.7, 0.5f, 1000); // 设置不合理视角下的投影参数
//  }
  /// Sample4_11 *************************************************** end

  /// Sample4_12 ************************************************* start
//  MatrixState3D::setCamera(5000.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
//  float NEAR;                                                             // 表示近平面参数的变量
//  if (ProjectPara) {
//    NEAR = 800.0f;                                                        // 较大的NEAR值
//  } else {
//    NEAR = 1.0f;                                                          // 较小的NEAR值
//  }
//  MatrixState3D::setProjectFrustum(
//      -NEAR * ratio * 0.25f, NEAR * ratio * 0.25f, -NEAR * 0.25f, NEAR * 0.25f, NEAR, 10000.0f);
  /// Sample4_12 *************************************************** end

  /// Sample4_13 ************************************************* start
//  MatrixState3D::setProjectFrustum(-ratio * 75.0f, ratio * 75.0f, -75.0f, 75.0f, 300.0f, 10000.0f);
////  MatrixState3D::setProjectFrustum(                                       // 效果更明显
////      -ratio * 225.0f, ratio * 225.0f, -225.0f, 225.0f, 900.0f, 10000.0f);
//  MatrixState3D::setCamera(5000.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  /// Sample4_13 *************************************************** end

  /// Sample5_5、Sample5_6
  LightManager::setLightPosition(0, 0, -13);  // 设置定位光光源位置
  LightManager::setLightDirection(-0.0f, 0.0f, 1.0f); // 设置定向光光源方向
  LightManager::setLightAmbient(0.1f, 0.1f, 0.1f, 0.1f);  // 设置环境光强度
  LightManager::setLightDiffuse(0.6f, 0.6f, 0.6f, 0.6f);  // 设置散射光强度
  LightManager::setLightSpecular(0.4f, 0.4f, 0.4f, 0.4f); // 设置镜面光强度
}

/**
 * 将当前帧相关数据送入一致变量缓冲
 */
void MyVulkanManager::flushUniformBuffer() {
//  xAngle = xAngle + 1.0f;                                                 // 改变3色三角形的旋转角
//  if (xAngle >= 360) { xAngle = 0; }                                      // 限制3色三角形旋转角范围
//
//  MatrixState3D::pushMatrix();                                            // 保护现场
//  MatrixState3D::rotate(xAngle, 1, 0, 0);                           // 旋转变换
//  float *vertexUniformData = MatrixState3D::getFinalMatrix();             // 获取最终变换矩阵
//  MatrixState3D::popMatrix();                                             // 恢复现场

  /// Sample4_14 ************************************************* start
//  if (xAngle >= 360) {
//    xAngle = 0;
//  }
//  if (yAngle >= 360) {
//    yAngle = 0;
//  }
//  MatrixState3D::pushMatrix();
//  MatrixState3D::rotate(xAngle, 1, 0, 0);
//  MatrixState3D::rotate(yAngle, 0, 1, 0);
  /// Sample4_14 *************************************************** end

  /// Sample4_2
//  float *vertexUniformData = MatrixState3D::getFinalMatrix();

  /// Sample4_14
//  MatrixState3D::popMatrix();

  /// Sample5_1-棋盘格球的两种颜色
//  float vertexUniformData[8] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0};

  /// Sample5_2-环境光
//  float vertexUniformData[4] = {LightManager::lightAmbientR, LightManager::lightAmbientG, LightManager::lightAmbientB,
//                                LightManager::lightAmbientA};

  /// Sample5_3-散射光
//  float vertexUniformData[8] = {
//      // 光源位置
//      LightManager::lx, LightManager::ly, LightManager::lz, 1.0,  // 注: 补了一个1.0为了使总的数据量为16字节的整数倍
//      // 散射光强度
//      LightManager::lightDiffuseR, LightManager::lightDiffuseG, LightManager::lightDiffuseB, LightManager::lightDiffuseA
//  };

  /// Sample5_4-镜面光
//  float vertexUniformData[12] = {                                         // 一致缓冲数据数组
//      MatrixState3D::cx, MatrixState3D::cy, MatrixState3D::cz, 1.0,       // 摄像机位置XYZ分量值
//      LightManager::lx, LightManager::ly, LightManager::lz, 1.0,          // 光源位置XYZ分量值
//      LightManager::lightSpecularR, LightManager::lightSpecularG,         // 镜面光强度RGBA分量值
//      LightManager::lightSpecularB, LightManager::lightSpecularA
//  };

  /// Sample5_5、Sample5_6-三光合一
  float vertexUniformData[20] = {
      MatrixState3D::cx, MatrixState3D::cy, MatrixState3D::cz, 1.0,
//      LightManager::lx, LightManager::ly, LightManager::lz, 1.0,          // Sample5_5-定位光
      LightManager::ldx, LightManager::ldy, LightManager::ldz, 1.0,       // Sample5_6-定向光
      LightManager::lightAmbientR, LightManager::lightAmbientG, LightManager::lightAmbientB,
      LightManager::lightAmbientA,
      LightManager::lightDiffuseR, LightManager::lightDiffuseG, LightManager::lightDiffuseB,
      LightManager::lightDiffuseA,
      LightManager::lightSpecularR, LightManager::lightSpecularG, LightManager::lightSpecularB,
      LightManager::lightSpecularA
  };

  uint8_t *pData;                                                         // CPU访问设备内存时的辅助指针
  VkResult result = vk::vkMapMemory(                                      // 将设备内存映射为CPU可访问
      device, sqsCL->memUniformBuf, 0, sqsCL->bufferByteCount, 0, (void **) &pData);
  assert(result == VK_SUCCESS);                                           // 检查映射是否成功
  memcpy(pData, vertexUniformData, sqsCL->bufferByteCount);               // 将最终矩阵数据复制进设备内存
  vk::vkUnmapMemory(device, sqsCL->memUniformBuf);                        // 解除内存映射
}

/**
 * 更新绘制用描述集
 */
void MyVulkanManager::flushTexToDesSet() {
  sqsCL->writes[0].dstSet = sqsCL->descSet[0];                            // 更新描述集对应的写入属性
  vk::vkUpdateDescriptorSets(device, 1, sqsCL->writes, 0, nullptr);       // 更新描述集
}

/**
 * 执行绘制
 * 其中建立了渲染循环以持续绘制各帧画面
 */
void MyVulkanManager::drawObject() {
  FPSUtil::init();                                                        // 初始化FPS计算

  /// Sample4_1 ************************************************** start
//  vpCenterX = screenWidth / 2;
//  vpCenterY = screenHeight / 2;
  /// Sample4_1 **************************************************** end

  while (MyVulkanManager::loopDrawFlag) {                                 // 每循环一次绘制一帧画面
    FPSUtil::calFPS();                                                    // 计算FPS
    FPSUtil::before();                                                    // 一帧开始

    VkResult result = vk::vkAcquireNextImageKHR(                          // 获取交换链中的当前帧索引
        device, swapChain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &currentBuffer);
    rp_begin.framebuffer = framebuffers[currentBuffer];                   // 为渲染通道设置此次绘制使用的帧缓冲索引
    vk::vkResetCommandBuffer(cmdBuffer, 0);                               // 恢复命令缓冲到初始状态
    result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);          // 启动命令缓冲

    MyVulkanManager::flushUniformBuffer();                                // 将当前帧相关数据送入一致变量缓冲
    MyVulkanManager::flushTexToDesSet();                                  // 更新绘制用描述集

    /// Sample4_1 ************************************************** start
//    VkViewport viewportList[1];                                           // 视口信息序列
//    viewportList[0].minDepth = 0.0f;                                      // 视口最小深度
//    viewportList[0].maxDepth = 1.0f;                                      // 视口最大深度
//    viewportList[0].x = vpCenterX - screenWidth / 4;                      // 视口X坐标
//    viewportList[0].y = vpCenterY - screenHeight / 4;                     // 视口Y坐标
//    viewportList[0].width = screenWidth / 2;                              // 视口宽度
//    viewportList[0].height = screenHeight / 2;                            // 视口高度
//    vk::vkCmdSetViewport(                                                 // 设置视口
//        cmdBuffer,                                                        // 命令缓冲
//        0,                                                                // 第1个视口的索引
//        1,                                                                // 视口的数量
//        viewportList);                                                    // 视口信息序列
    /// Sample4_1 **************************************************** end

    // VK_SUBPASS_CONTENTS_INLINE：表示仅采用主命令缓冲而没有采用二级命令缓冲(或称之为子命令缓冲)
    // 若需要采用二级命令缓冲，则第三个参数应该选用VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
    vk::vkCmdBeginRenderPass(cmdBuffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE); // 启动渲染通道

    /// Sample4_2 ************************************************** start
//    MatrixState3D::pushMatrix();                                          // 保护现场
//    MatrixState3D::rotate(xAngle, 1, 0, 0);                       // 绕x轴旋转xAngle
//    MatrixState3D::rotate(yAngle, 0, 1, 0);                       // 绕y轴旋转yAngle
//    for (int i = 0; i <= 5; ++i) {                                        // 循环绘制所有六角星
//      MatrixState3D::pushMatrix();                                        // 保护现场
//      MatrixState3D::translate(0, 0, i * 0.5);                    // 沿Z轴平移
//      objForDraw->drawSelf(                                               // 绘制物体
//          cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//      MatrixState3D::popMatrix();                                         // 恢复现场
//    }
//    MatrixState3D::popMatrix();                                           // 恢复现场
    /// Sample4_2 **************************************************** end

    /// Sample4_4 ************************************************** start
//    MatrixState3D::pushMatrix();
//    MatrixState3D::rotate(xAngle, 1, 0, 0);
//    MatrixState3D::rotate(yAngle, 0, 1, 0);
//    MatrixState3D::pushMatrix();
//    objForDraw->drawSelf(                                                 // 绘制第一个立方体
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(3.5f, 0, 0);                         // Sample4_4-沿x方向平移3.5
//    MatrixState3D::rotate(30, 0, 0, 1);                      // Sample4_5-绕z轴旋转30°
//    MatrixState3D::scale(0.4f, 2.0f, 0.6f);                       // Sample4_6-x轴、y轴、z轴3个方向按各自的缩放因子进行缩放
//    objForDraw->drawSelf(                                                 // 绘制变换后的立方体
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//    MatrixState3D::popMatrix();
    /// Sample4_4 **************************************************** end

    /// Sample4_7 ************************************************** start
//    objForDraw->drawSelf(
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline[topologyWay], &(sqsCL->descSet[0]));
    /// Sample4_7 **************************************************** end

    /// Sample4_8 ************************************************** start
//    MatrixState3D::pushMatrix();
//    MatrixState3D::rotate(xAngle, 1, 0, 0);
//    MatrixState3D::rotate(yAngle, 0, 1, 0);
//
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(90, 0, 0);
//    triForDraw->drawSelf(                                                 // 绘制三角形条带
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline[0], &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(-90, 0, 0);
//    cirForDraw->drawSelf(                                                 // 绘制扇形
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline[1], &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//
//    MatrixState3D::popMatrix();
    /// Sample4_8 **************************************************** end

    /// Sample4_10、Sample4_16 ************************************** start
//    MatrixState3D::pushMatrix();
//    MatrixState3D::rotate(xAngle, 1, 0, 0);
//    MatrixState3D::rotate(yAngle, 0, 1, 0);
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(0, 50, 0);
////    cirForDraw->drawSelf(                                                 // 绘制正十边形
////        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]), 0, CircleData::iCount);
//    cirForDraw->drawSelf(                                                 // Sample4_16-间接绘制正十边形
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]), 0);
//    MatrixState3D::popMatrix();
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(0, -50, 0);
////    cirForDraw->drawSelf(                                                 // 绘制正十边形的下半部分
////        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]), 0, CircleData::iCount / 2 + 1);
//    cirForDraw->drawSelf(                                                 // Sample4_16-间接绘制正十边形的下半部分
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]), sizeof(VkDrawIndexedIndirectCommand));
//    MatrixState3D::popMatrix();
//    MatrixState3D::popMatrix();
    /// Sample4_10、Sample4_16 *************************************** end

    /// Sample4_11 ************************************************* start
//    MatrixState3D::pushMatrix();
//    if (yAngle >= 360) {
//      yAngle = -360;
//    }
//    MatrixState3D::rotate(yAngle, 0, 1, 0);
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(-80, 0, 0);
//    MatrixState3D::rotate(-30, 0, 1, 0);
//    cubeForDraw->drawSelf(                                                // 绘制第一个立方体
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(80, 0, 0);
//    MatrixState3D::rotate(30, 0, 1, 0);
//    cubeForDraw->drawSelf(                                                // 绘制第二个立方体
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//    MatrixState3D::popMatrix();
    /// Sample4_11 *************************************************** end

    /// Sample4_12 ************************************************* start
//    MatrixState3D::pushMatrix();
//    MatrixState3D::rotate(-yAngle, 0, 1, 0);
//    MatrixState3D::rotate(-zAngle, 0, 0, 1);
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(250, 0, 0);
//    cube1ForDraw->drawSelf(cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(-250, 0, 0);
//    cube2ForDraw->drawSelf(cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//    MatrixState3D::popMatrix();
    /// Sample4_12 *************************************************** end

    /// Sample4_13 ************************************************* start
//    MatrixState3D::pushMatrix();
//    MatrixState3D::rotate(xAngle, 1, 0, 0);
//    MatrixState3D::rotate(yAngle, 0, 1, 0);
//    vk::vkCmdSetDepthBias(cmdBuffer, 0.0, 0.0, 0.0);                      // 设置青色矩形的深度偏移信息
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(-250.0f, 0.0f, 0.0f);
//    colorRectG->drawSelf(cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//
//    switch (depthOffsetFlag) {                                            // 根据索引设置黄色矩形深度偏移参数
//      case 1:vk::vkCmdSetDepthBias(cmdBuffer, -1.0, -3.0, -2.0);          // 黄色矩形深度值减小
//        break;
//      case 2:vk::vkCmdSetDepthBias(cmdBuffer, 1.0, 3.0, 2.0);             // 黄色矩形深度值增大
//        break;
//      default:break;
//    }
//    MatrixState3D::pushMatrix();
//    MatrixState3D::translate(250.0f, 0.0f, 0.0f);
//    colorRectY->drawSelf(cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
//    MatrixState3D::popMatrix();
    /// Sample4_13 *************************************************** end

    /// Sample5_1 ************************************************** start
//    MatrixState3D::pushMatrix();
//    MatrixState3D::rotate(xAngle, 1, 0, 0);
//    MatrixState3D::rotate(yAngle, 0, 1, 0);
//    ballForDraw->drawSelf(cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
//    MatrixState3D::popMatrix();
    /// Sample5_1 **************************************************** end

    /// Sample5_2 ************************************************** start
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(-1.5f, 0, -15);
    ballForDraw->drawSelf(cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
    MatrixState3D::popMatrix();
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(1.5f, 0, -15);
    ballForDraw->drawSelf(cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
    MatrixState3D::popMatrix();
    /// Sample5_2 **************************************************** end

//    triForDraw->drawSelf(                                                 // 绘制三色三角形、Sample4_14-卷绕和背面剪裁
//        cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline, &(sqsCL->descSet[0]));
    vk::vkCmdEndRenderPass(cmdBuffer);                                    // 结束渲染通道
    result = vk::vkEndCommandBuffer(cmdBuffer);                           // 结束命令缓冲

    submit_info[0].waitSemaphoreCount = 1;                                // 等待的信号量数量
    // 该信号量是前面获取交换链中当前帧索引时设置的。
    // 这样命令缓冲提交后，在执行前就会等待此信号量到位，避免多个队列同时执行导致的并发问题
    submit_info[0].pWaitSemaphores = &imageAcquiredSemaphore;             // 等待的信号量列表
    result = vk::vkQueueSubmit(queueGraphics, 1, submit_info, taskFinishFence); // 提交命令缓冲到指定的队列执行并指定栅栏

    do {
      result = vk::vkWaitForFences(device, 1, &taskFinishFence, VK_TRUE, FENCE_TIMEOUT); // 循环等待提交到队列的任务完成
    } while (result == VK_TIMEOUT);                                       // 等待渲染完毕
    vk::vkResetFences(device, 1, &taskFinishFence);                       // 重置栅栏

    present.pImageIndices = &currentBuffer;                               // 指定此次呈现的交换链图像索引
    result = vk::vkQueuePresentKHR(queueGraphics, &present);              // 执行呈现(执行完毕后，就可以看到一帧完整的画面了)

    FPSUtil::after(60);                                             // 限制FPS不超过指定的值
  }
}

/**
 * 初始化渲染管线
 */
void MyVulkanManager::initPipeline() {
  sqsCL = new ShaderQueueSuit_Common(&device, renderPass, memoryroperties); // 创建封装了渲染管线相关的对象
}

/**
 * 销毁管线
 */
void MyVulkanManager::destroyPipeline() {
  delete sqsCL;
}

/**
 * 启动自定义线程执行Vulkan绘制相关任务
 */
void MyVulkanManager::doVulkan() {
  auto *tt = new ThreadTask();                                    // 创建执行Vulkan绘制相关任务的对象
  thread t1(&ThreadTask::doTask, tt);                             // 创建线程执行任务方法doTask
  t1.detach();                                                    // 将子线程与主线程分离
}
