#ifndef DEEPERVULKAN_MYVULKANMANAGER_H
#define DEEPERVULKAN_MYVULKANMANAGER_H

#include <android_native_app_glue.h>
#include <vector>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include "mylog.h"
#include "DrawableObjectCommon.h"
#include "ShaderQueueSuit_Common.h"
#include "Cube.h"
#include "TexDataObject.h"

#define FENCE_TIMEOUT 100000000                           // 栅栏的超时时间

class MyVulkanManager {
 public:
  static android_app *Android_application;                // Android应用指针
  static bool loopDrawFlag;                               // 绘制的循环工作标志
  static std::vector<const char *> instanceExtensionNames;// 需要使用的实例扩展名称列表
  static VkInstance instance;                             // Vulkan实例
  static uint32_t gpuCount;                               // 物理设备数量
  static std::vector<VkPhysicalDevice> gpus;              // 物理设备列表
  static uint32_t queueFamilyCount;                       // 物理设备对应的队列家族数量
  static std::vector<VkQueueFamilyProperties> queueFamilyprops; // 物理设备对应的队列家族属性列表
  static uint32_t queueGraphicsFamilyIndex;               // 支持图形工作的队列家族索引
  static VkQueue queueGraphics;                           // 支持图形工作的队列
  static uint32_t queuePresentFamilyIndex;                // 支持显示工作的队列家族索引
  static std::vector<const char *> deviceExtensionNames;  // 所需的设备扩展名称列表
  static VkDevice device;                                 // 逻辑设备
  static VkCommandPool cmdPool;                           // 命令池
  static VkCommandBuffer cmdBuffer;                       // 命令缓冲
  static VkCommandBufferBeginInfo cmd_buf_info;           // 命令缓冲启动信息
  static VkCommandBuffer cmd_bufs[1];                     // 供提交执行的命令缓冲数组
  static VkSubmitInfo submit_info[1];                     // 命令缓冲提交执行信息数组
  static uint32_t screenWidth;                            // 屏幕宽度
  static uint32_t screenHeight;                           // 屏幕高度
  static VkSurfaceKHR surface;                            // KHR表面
  static std::vector<VkFormat> formats;                   // KHR表面支持的格式
  static VkSurfaceCapabilitiesKHR surfCapabilities;       // 表面的能力
  static uint32_t presentModeCount;                       // 显示模式数量
  static std::vector<VkPresentModeKHR> presentModes;      // 显示模式列表
  static VkExtent2D swapchainExtent;                      // 交换链尺寸
  static VkSwapchainKHR swapChain;                        // 交换链
  static uint32_t swapchainImageCount;                    // 交换链中的图像数量
  static std::vector<VkImage> swapchainImages;            // 交换链中的图像列表
  static std::vector<VkImageView> swapchainImageViews;    // 交换链对应的图像视图列表
  static VkFormat depthFormat;                            // 深度图像格式
  static VkFormatProperties depthFormatProps;             // 物理设备支持的深度格式属性
  static VkImage depthImage;                              // 深度缓冲图像
  static VkPhysicalDeviceMemoryProperties memoryroperties;// 物理设备内存属性
  static VkDeviceMemory memDepth;                         // 深度缓冲图像对应的内存
  static VkImageView depthImageView;                      // 深度缓冲图像视图
  static VkSemaphore imageAcquiredSemaphore;              // 渲染目标图像获取完成信号量
  static uint32_t currentBuffer;                          // 从交换链中获取的当前渲染用图像对应的缓冲编号
  static VkRenderPass renderPass;                         // 渲染通道
  static VkClearValue clear_values[2];                    // 渲染通道用清除帧缓冲深度、颜色附件的数据
  static VkRenderPassBeginInfo rp_begin;                  // 渲染通道启动信息
  static VkFence taskFinishFence;                         // 等待任务完毕的栅栏
  static VkPresentInfoKHR present;                        // 呈现信息
  static VkFramebuffer *framebuffers;                     // 帧缓冲序列首指针
  static ShaderQueueSuit_Common *sqsCL;                   // 着色器管线指针
  static DrawableObjectCommon *triForDraw;                // Sample4_1-绘制用3色三角形物体对象指针
  static DrawableObjectCommon *objForDraw;                // Sample4_2-六角星
  static float xAngle;                                    // 三角形旋转角度
  /// Sample4_2 六角星
  static float yAngle;

  /// Sample4_1 触摸点作为显示中点
  static int vpCenterX;
  static int vpCenterY;

  /// Sample4_7 线段绘制方式
  static int topologyWay;

  /// Sample4_8 扇形条
  static DrawableObjectCommon *cirForDraw;

  /// Sample4_11 视角立方体
  static Cube *cubeForDraw;
  static int ViewPara;

  /// Sample4_12 远视角造成深度遮挡误差
  static Cube *cube1ForDraw;
  static Cube *cube2ForDraw;
  static int ProjectPara;
  static float zAngle;

  /// Sample4_13 深度偏移
  static int depthOffsetFlag;
  static DrawableObjectCommon *colorRectG;
  static DrawableObjectCommon *colorRectY;

  /// Sample5_1 球
  static DrawableObjectCommon *ballForDraw;

  /// Sample6_1 纹理
  static DrawableObjectCommon * texTri;

  static void init_vulkan_instance();                     // 创建Vulkan实例
  static void enumerate_vulkan_phy_devices();             // 初始化物理设备
  static void create_vulkan_devices();                    // 创建逻辑设备
  static void create_vulkan_CommandBuffer();              // 创建命令缓冲
  static void create_vulkan_swapChain();                  // 初始化交换链
  static void create_vulkan_DepthBuffer();                // 创建深度缓冲相关
  static void create_render_pass();                       // 创建渲染通道
  static void init_queue();                               // 获取设备中支持图形工作的队列
  static void create_frame_buffer();                      // 创建帧缓冲
  static void init_texture();                             // Sample6_1-初始化纹理
  static void createDrawableObject();                     // 创建绘制用物体
  static void drawObject();                               // 执行场景中的物体绘制
  static void doVulkan();                                 // 启动线程执行Vulkan任务
  static void initPipeline();                             // 初始化管线
  static void createFence();                              // 创建栅栏
  static void initPresentInfo();                          // 初始化显示信息
//  static void initMatrix();                               // 初始化矩阵
  static void initMatrixAndLight();                       // Sample5_2-初始化矩阵和光照
  static void flushUniformBuffer();                       // 将一致变量数据送入缓冲
  static void flushTexToDesSet();                         // 将纹理等数据与描述集关联
  static void destroyFence();                             // 销毁栅栏
  static void destroyPipeline();                          // 销毁管线
  static void destroyDrawableObject();                    // 销毁绘制用物体
  static void destroy_textures();                         // 销毁纹理
  static void destroy_frame_buffer();                     // 销毁帧缓冲
  static void destroy_render_pass();                      // 销毁渲染通道
  static void destroy_vulkan_DepthBuffer();               // 销毁深度缓冲相关
  static void destroy_vulkan_swapChain();                 // 销毁交换链
  static void destroy_vulkan_CommandBuffer();             // 销毁命令缓冲
  static void destroy_vulkan_devices();                   // 销毁逻辑设备
  static void destroy_vulkan_instance();                  // 销毁实例
};

#endif //DEEPERVULKAN_MYVULKANMANAGER_H
