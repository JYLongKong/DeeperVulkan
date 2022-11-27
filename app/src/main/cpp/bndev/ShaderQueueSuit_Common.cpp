#include "ShaderQueueSuit_Common.h"
#include <assert.h>
#include "mylog.h"
#include "MyVulkanManager.h"
#include "ShaderCompileUtil.h"
#include "../util/HelpFunction.h"
#include "../util/FileUtil.h"
#include "../util/TextureManager.h"

ShaderQueueSuit_Common::ShaderQueueSuit_Common(VkDevice *deviceIn,
                                               VkRenderPass &renderPass,
                                               VkPhysicalDeviceMemoryProperties &memoryroperties) {
  this->devicePointer = deviceIn;
  create_uniform_buffer(*devicePointer, memoryroperties);           // 创建一致变量缓冲
  create_pipeline_layout(*devicePointer);                              // 创建管线布局
  init_descriptor_set(*devicePointer);                                 // 初始化描述集
  create_shader(*devicePointer);                                       // 创建着色器
  initVertexAttributeInfo();                                              // 初始化顶点属性信息
  create_pipe_line(*devicePointer, renderPass);                      // 创建管线
}

ShaderQueueSuit_Common::~ShaderQueueSuit_Common() {
  destroy_pipe_line(*devicePointer);                                    // 销毁管线
  destroy_shader(*devicePointer);                                       // 销毁着色器模块
  destroy_pipeline_layout(*devicePointer);                              // 销毁管线布局
  destroy_uniform_buffer(*devicePointer);                               // 销毁一致变量缓冲
}

/**
 * 创建一致变量缓冲
 */
void ShaderQueueSuit_Common::create_uniform_buffer(VkDevice &device,
                                                   VkPhysicalDeviceMemoryProperties &memoryroperties) {
  // 计算一致变量缓冲的总字节数，与后面着色器中对应的一致变量块所占的总字节数是一致的
  // 当着色器的这部分发生变化时，这里也需要相应修改(本案例用于存储总变换矩阵4x4)
//  bufferByteCount = sizeof(float) * 16;
//  bufferByteCount = sizeof(float) * 8;                                    // Sample5_1
//  bufferByteCount = sizeof(float) * 4;                                    // Sample5_2
//  bufferByteCount = sizeof(float) * 8;                                    // Sample5_3-此处的总字节数应与vertexUniformData一致
//  bufferByteCount = sizeof(float) * 12;                                   // Sample5_4
//  bufferByteCount = sizeof(float) * 20;                                   // Sample5_5
  bufferByteCount = sizeof(float);                                        // Sample6_1

  VkBufferCreateInfo buf_info = {};                                       // 构建一致变量缓冲创建信息结构体实例
  buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;                  // 结构体的类型
  buf_info.pNext = nullptr;                                               // 自定义数据的指针
  buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;                    // 缓冲的用途
  buf_info.size = bufferByteCount;                                        // 缓冲总字节数
  buf_info.queueFamilyIndexCount = 0;                                     // 队列家族数量
  buf_info.pQueueFamilyIndices = nullptr;                                 // 队列家族索引列表
  buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;                       // 共享模式
  buf_info.flags = 0;                                                     // 标志

  VkResult result = vk::vkCreateBuffer(device, &buf_info, nullptr, &uniformBuf); // 创建一致变量缓冲
  assert(result == VK_SUCCESS);                                           // 检查创建是否成功

  VkMemoryRequirements mem_reqs;                                          // 内存需求变量
  vk::vkGetBufferMemoryRequirements(device, uniformBuf, &mem_reqs);       // 获取此缓冲的内存需求

  VkMemoryAllocateInfo alloc_info = {};                                   // 构建内存分配信息结构体实例
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;              // 结构体类型
  alloc_info.pNext = nullptr;                                             // 自定义数据的指针
  alloc_info.memoryTypeIndex = 0;                                         // 内存类型索引
  alloc_info.allocationSize = mem_reqs.size;                              // 缓冲内存分配字节数

  VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |       // 需要的内存类型掩码
      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;                               // 该组合表示分配的设备内存可以被CPU访问，同时保证CPU与GPU访问的一致性
  bool flag = memoryTypeFromProperties(                                   // 获取所需内存类型索引
      memoryroperties, mem_reqs.memoryTypeBits, requirements_mask, &alloc_info.memoryTypeIndex);

  if (flag) {
    LOGI("confirm memory type of uniformBuf succeed, memoryTypeIndex = %d", alloc_info.memoryTypeIndex);
  } else {
    LOGE("confirm memory type of uniformBuf failed!");
  }

  result = vk::vkAllocateMemory(device, &alloc_info, nullptr, &memUniformBuf); // 分配内存
  assert(result == VK_SUCCESS);                                           // 检查内存分配是否成功

  result = vk::vkBindBufferMemory(device, uniformBuf, memUniformBuf, 0);  // 将内存和一致变量缓冲绑定
  assert(result == VK_SUCCESS);                                           // 检查绑定操作是否成功

  // 完善了一致变量缓冲信息结构体实例，为后面对缓冲的使用做好准备
  uniformBufferInfo.buffer = uniformBuf;                                  // 指定一致变量缓冲
  uniformBufferInfo.offset = 0;                                           // 起始偏移量
  uniformBufferInfo.range = bufferByteCount;                              // 一致变量缓冲总字节数
}

/**
 * 销毁一致变量缓冲并释放对应设备内存
 */
void ShaderQueueSuit_Common::destroy_uniform_buffer(VkDevice &device) {
  vk::vkDestroyBuffer(device, uniformBuf, nullptr);
  vk::vkFreeMemory(device, memUniformBuf, nullptr);
}

/**
 * 创建管线布局
 *
 * 管线布局主要是管理相关的各个描述集，而描述集负责将所需的一致数据、纹理等资源与管线关联，以备特定着色器进行访问。
 * 每个描述集布局关联多个描述集布局绑定，每个描述集布局绑定关联到某个着色器阶段着色器中的某项一致数据或纹理采样器等。
 * 描述集布局绑定与着色器中接收的一致变量情况应当是匹配的。
 */
void ShaderQueueSuit_Common::create_pipeline_layout(VkDevice &device) {
  NUM_DESCRIPTOR_SETS = 1;                                                // 设置描述集数量

//  VkDescriptorSetLayoutBinding layout_bindings[1];                        // 描述集布局绑定数组
  VkDescriptorSetLayoutBinding layout_bindings[2];                        // Sample6_1
  layout_bindings[0].binding = 0;                                         // 此绑定的绑定点编号(需要与着色器中给定的对应绑定点编号一致)
  layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;  // 描述类型(此绑定对应类型为一致变量缓冲)
  layout_bindings[0].descriptorCount = 1;                                 // 描述数量
//  layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;             // 目标着色器阶段(此绑定对应的是顶点着色器)
  layout_bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;           // Sample5_1、5_9、6_1-目标着色器阶段(此绑定对应的是片元着色器)
  layout_bindings[0].pImmutableSamplers = nullptr;

  /// Sample6_1 ************************************************** start
  layout_bindings[1].binding = 1;                                         // 此绑定的绑定点编号为1
  layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // 此描述集布局绑定的对应的类型为纹理采样器
  layout_bindings[1].descriptorCount = 1;                                 // 描述数量
  layout_bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;           // 目标着色器阶段
  layout_bindings[1].pImmutableSamplers = nullptr;
  /// Sample6_1 **************************************************** end

  VkDescriptorSetLayoutCreateInfo descriptor_layout = {};                 // 构建描述集布局创建信息结构体实例
  descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  descriptor_layout.pNext = nullptr;
//  descriptor_layout.bindingCount = 1;                                     // 描述集布局绑定的数量
  descriptor_layout.bindingCount = 2;                                     // Sample6_1
  descriptor_layout.pBindings = layout_bindings;                          // 描述集布局绑定数组

  descLayouts.resize(NUM_DESCRIPTOR_SETS);                                // 调整描述集布局列表尺寸
  VkResult result = vk::vkCreateDescriptorSetLayout(                      // 创建描述集布局
      device, &descriptor_layout, nullptr, descLayouts.data());
  assert(result == VK_SUCCESS);                                           // 检查描述集布局创建是否成功

  /// Sample4_2、5_2、6_1、6_6、6_10 ******************************* start
//  const unsigned push_constant_range_count = 1;                           // 推送常量块数量
//  VkPushConstantRange push_constant_ranges[push_constant_range_count] = {}; // 推送常量范围列表
//  push_constant_ranges[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;        // 对应着色器阶段
//  push_constant_ranges[0].offset = 0;                                     // 推送常量数据起始偏移量
////  push_constant_ranges[0].size = sizeof(float) * 16;                      // 推送常量数据总字节数
////  push_constant_ranges[0].size = sizeof(float) * 32;                      // Sample5_2
//  push_constant_ranges[0].size = sizeof(float) * 17;                      // Sample6_10
  /// Sample4_2、5_2、6_1、6_6、6_10 ******************************** end

  /// Sample6_5、Sample6_11 *************************************** start
  const unsigned push_constant_range_count = 2;
  VkPushConstantRange push_constant_ranges[push_constant_range_count] = {};
  push_constant_ranges[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  push_constant_ranges[0].offset = 0;
  push_constant_ranges[0].size = sizeof(float) * 16;
  push_constant_ranges[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  push_constant_ranges[1].offset = sizeof(float) * 16;
  push_constant_ranges[1].size = sizeof(float) * 1;
  /// Sample6_5、Sample6_11 ***************************************** end

  VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};              // 构建管线布局创建信息结构体实例
  pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pPipelineLayoutCreateInfo.pNext = nullptr;
//  pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;                   // 推送常量范围的数量
//  pPipelineLayoutCreateInfo.pPushConstantRanges = nullptr;                // 推送常量范围的列表
  pPipelineLayoutCreateInfo.pushConstantRangeCount = push_constant_range_count; // Sample4_2-推送常量范围数量
  pPipelineLayoutCreateInfo.pPushConstantRanges = push_constant_ranges;   // Sample4_2-推送常量范围列表
  pPipelineLayoutCreateInfo.setLayoutCount = NUM_DESCRIPTOR_SETS;         // 描述集布局的数量
  pPipelineLayoutCreateInfo.pSetLayouts = descLayouts.data();             // 描述集布局列表

  result = vk::vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, nullptr, &pipelineLayout); // 创建管线布局
  assert(result == VK_SUCCESS);
}

/**
 * 遍历销毁描述集布局列表并销毁管线布局
 */
void ShaderQueueSuit_Common::destroy_pipeline_layout(VkDevice &device) {
  for (int i = 0; i < NUM_DESCRIPTOR_SETS; i++) {
    vk::vkDestroyDescriptorSetLayout(device, descLayouts[i], nullptr);
  }
  vk::vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}

/**
 * 初始化描述集
 */
void ShaderQueueSuit_Common::init_descriptor_set(VkDevice &device) {
//  VkDescriptorPoolSize type_count[1];                                     // 描述集池尺寸实例数组
//  type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;                 // 描述类型(一致变量缓冲)
//  type_count[0].descriptorCount = 1;                                      // 描述数量

  /// Sample6_1 ************************************************** start
  VkDescriptorPoolSize type_count[2];                                     // 描述集池尺寸实例数组
  type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;                 // 第1个描述类型
  type_count[0].descriptorCount = TextureManager::texNames.size();        // 第1个描述数量
  type_count[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;         // 第2个描述类型
  type_count[1].descriptorCount = TextureManager::texNames.size();        // 第2个描述数量
  /// Sample6_1 **************************************************** end

  VkDescriptorPoolCreateInfo descriptor_pool = {};                        // 构建描述集池创建信息结构体实例
  descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptor_pool.pNext = nullptr;
//  descriptor_pool.maxSets = 1;                                            // 描述集最大数量
//  descriptor_pool.poolSizeCount = 1;                                      // 描述集池尺寸实例数量
  descriptor_pool.maxSets = TextureManager::texNames.size();              // Sample6_1
  descriptor_pool.poolSizeCount = 2;                                      // Sample6_1
  descriptor_pool.pPoolSizes = type_count;                                // 描述集池尺寸实例数组
  VkResult result = vk::vkCreateDescriptorPool(device, &descriptor_pool, nullptr, &descPool); // 创建描述集池
  assert(result == VK_SUCCESS);                                           // 检查描述集池创建是否成功

  std::vector<VkDescriptorSetLayout> layouts;                             // 描述集布局列表
//  layouts.push_back(descLayouts[0]);                                      // 向列表中添加指定描述集布局
  for (int i = 0; i < TextureManager::texNames.size(); ++i) {             // Sample6_1-遍历所有纹理
    layouts.push_back(descLayouts[0]);                                    // Sample6_1-向列表中添加指定描述集布局
  }

  VkDescriptorSetAllocateInfo alloc_info[1];                              // 构建描述集分配信息结构体实例数组
  alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  alloc_info[0].pNext = nullptr;
  alloc_info[0].descriptorPool = descPool;                                // 指定描述集池
//  alloc_info[0].descriptorSetCount = 1;                                   // 描述集数量
  alloc_info[0].descriptorSetCount = TextureManager::texNames.size();     // Sample6_1
  alloc_info[0].pSetLayouts = layouts.data();                             // 描述集布局列表
//  descSet.resize(1);                                                   // 调整描述集列表尺寸
  descSet.resize(TextureManager::texNames.size());                        // Sample6_1
  result = vk::vkAllocateDescriptorSets(device, alloc_info, descSet.data()); // 分配指定数量的描述集
  assert(result == VK_SUCCESS);                                           // 检查描述集分配是否成功

  writes[0] = {};                                                         // 完善一致变量写入描述集实例数组元素0
  writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  writes[0].pNext = nullptr;
  writes[0].descriptorCount = 1;                                          // 描述数量
  writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;           // 描述类型(一致变量缓冲)
  writes[0].pBufferInfo = &uniformBufferInfo;                             // 对应一致变量缓冲的信息
  writes[0].dstArrayElement = 0;                                          // 目标数组起始元素
  writes[0].dstBinding = 0;                                               // 目标绑定编号(与着色器中绑定编号对应)

  /// Sample6_1
  writes[1] = {};
  writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  writes[1].dstBinding = 1;
  writes[1].descriptorCount = 1;
  writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;   // 描述类型(采样用纹理)
  writes[1].dstArrayElement = 0;
}

/**
 * 创建着色器
 */
void ShaderQueueSuit_Common::create_shader(VkDevice &device) {
//  std::string vertStr = FileUtil::loadAssetStr("shader/commonTexLight.vert"); // 加载顶点着色器脚本
//  std::string fragStr = FileUtil::loadAssetStr("shader/commonTexLight.frag"); // 加载片元着色器脚本
//  std::string fragStr = FileUtil::loadAssetStr("shader/sample4_11.frag"); // Sample4_11-加载片元着色器脚本
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample5_1.vert");  // Sample5_1
//  std::string fragStr = FileUtil::loadAssetStr("shader/sample5_1.frag");  // Sample5_1
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample5_2.vert");  // Sample5_2
//  std::string fragStr = FileUtil::loadAssetStr("shader/sample5_2.frag");  // Sample5_2
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample5_3.vert");  // Sample5_3
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample5_4.vert");  // Sample5_4
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample5_5.vert");  // Sample5_5、Sample5_7、Sample5_10
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample5_6.vert");  // Sample5_6
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample5_9.vert");  // Sample5_9
//  std::string fragStr = FileUtil::loadAssetStr("shader/sample5_9.frag");  // Sample5_9
//  std::string fragStr = FileUtil::loadAssetStr("shader/sample5_10.frag"); // Sample5_10
  std::string vertStr = FileUtil::loadAssetStr("shader/sample6_1.vert");  // Sample6_1、Sample6_5、Sample6_7、Sample6_11
//  std::string fragStr = FileUtil::loadAssetStr("shader/sample6_1.frag");  // Sample6_1、Sample6_7
  std::string fragStr = FileUtil::loadAssetStr("shader/sample6_5.frag");  // Sample6_5、Sample6_11
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample6_6-star.vert"); // Sample6_6
//  std::string fragStr = FileUtil::loadAssetStr("shader/commonTexLight.frag"); // Sample6_6
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample6_8.vert");  // Sample6_8
//  std::string fragStr = FileUtil::loadAssetStr("shader/sample6_8.frag");  // Sample6_8
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample6_9.vert");  // Sample6_9
//  std::string fragStr = FileUtil::loadAssetStr("shader/sample6_9.frag");  // Sample6_9
//  std::string vertStr = FileUtil::loadAssetStr("shader/sample6_10.vert");  // Sample6_10
//  std::string fragStr = FileUtil::loadAssetStr("shader/sample6_10.frag");  // Sample6_10

  // 给出顶点着色器对应的管线着色器阶段创建信息结构体实例的各项所需属性
  shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStages[0].pNext = nullptr;
  shaderStages[0].pSpecializationInfo = nullptr;                          // 特殊信息
  shaderStages[0].flags = 0;                                              // 供将来使用的标志
  shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;                     // 着色器阶段为顶点
  shaderStages[0].pName = "main";                                         // 入口函数为main

  std::vector<unsigned int> vtx_spv;                                      // 用于存储编译后SPIR-V代码的列表
  bool retVal = GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, vertStr.c_str(), vtx_spv); // 将顶点着色器脚本编译成SPIR-V格式
  assert(retVal);                                                         // 检查编译是否成功
  LOGI("vertex shader compile success");

  VkShaderModuleCreateInfo moduleCreateInfo;                              // 准备顶点着色器模块创建信息结构体实例
  moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  moduleCreateInfo.pNext = nullptr;
  moduleCreateInfo.flags = 0;                                             // 供将来使用的标志
  moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);      // 顶点着色器SPV数据总字节数
  moduleCreateInfo.pCode = vtx_spv.data();                                // 顶点着色器SPV数据
  VkResult result = vk::vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &shaderStages[0].module); // 创建顶点着色器模块
  assert(result == VK_SUCCESS);                                           // 检查顶点着色器模块创建是否成功

  // 给出片元着色器对应的管线着色器阶段创建信息结构体实例的各项所需属性
  shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStages[1].pNext = nullptr;
  shaderStages[1].pSpecializationInfo = nullptr;                          // 特殊信息
  shaderStages[1].flags = 0;                                              // 供将来使用的标志
  shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;                   // 着色器阶段为片元
  shaderStages[1].pName = "main";                                         // 入口函数为main

  std::vector<unsigned int> frag_spv;
  retVal = GLSLtoSPV(VK_SHADER_STAGE_FRAGMENT_BIT, fragStr.c_str(), frag_spv); // 将片元着色器脚本编译为SPV
  assert(retVal);                                                         // 检查编译是否成功
  LOGI("fragment shader compile success");

  moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;   // 准备片元着色器模块创建信息结构体实例
  moduleCreateInfo.pNext = nullptr;
  moduleCreateInfo.flags = 0;                                             // 供将来使用的标志
  moduleCreateInfo.codeSize = frag_spv.size() * sizeof(unsigned int);     // 片元着色器SPV数据总字节数
  moduleCreateInfo.pCode = frag_spv.data();                               // 片元着色器SPV数据
  result = vk::vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &shaderStages[1].module); // 创建片元着色器模块
  assert(result == VK_SUCCESS);                                           // 检查片元着色器模块创建是否成功
}

/**
 * 销毁(顶点和片元)着色器模块
 */
void ShaderQueueSuit_Common::destroy_shader(VkDevice &device) {
  vk::vkDestroyShaderModule(device, shaderStages[0].module, nullptr);
  vk::vkDestroyShaderModule(device, shaderStages[1].module, nullptr);
}

/**
 * 设置顶点着色器输入属性信息
 */
void ShaderQueueSuit_Common::initVertexAttributeInfo() {
  // 设置顶点输入绑定描述结构体属性
  vertexBinding.binding = 0;                                              // 对应绑定点
  vertexBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;                  // 数据输入频率为每顶点输入一套数据
//  vertexBinding.stride = sizeof(float) * 6;                               // 每组数据的跨度字节数(x,y,z,R,G,B 6个分量)
//  vertexBinding.stride = sizeof(float) * 3;                               // Sample5_1-球
//  vertexBinding.stride = sizeof(float) * 6;                               // Sample5_3-顶点+法向共6个分量
  vertexBinding.stride = sizeof(float) * 5;                               // Sample6_1、6_7、6_10-顶点+纹理共5个分量
//  vertexBinding.stride = sizeof(float) * 6;                               // Sample6_6
//  vertexBinding.stride = sizeof(float) * 3;                               // Sample6_8

  vertexAttribs[0].binding = 0;                                           // 第1个顶点输入属性的绑定点
  vertexAttribs[0].location = 0;                                          // 第1个顶点输入属性的位置索引
  vertexAttribs[0].format = VK_FORMAT_R32G32B32_SFLOAT;                   // 第1个顶点输入属性的数据格式
  vertexAttribs[0].offset = 0;                                            // 第1个顶点输入属性的偏移量

//  vertexAttribs[1].binding = 0;                                           // 第2个顶点输入属性的绑定点
//  vertexAttribs[1].location = 1;                                          // 第2个顶点输入属性的位置索引
//  vertexAttribs[1].format = VK_FORMAT_R32G32B32_SFLOAT;                   // 第2个顶点输入属性的数据格式
//  // 由于第1个顶点输入属性包含3个float分量，每个float分量4个字节，偏移量以字节计
//  vertexAttribs[1].offset = 12;                                           // 第2个顶点输入属性的偏移量

  /// Sample5_3、Sample6_6 *************************************** start
//  vertexAttribs[1].binding = 0;                                           // 法向量输入属性的绑定点
//  vertexAttribs[1].location = 1;                                          // 法向量输入属性的位置索引
//  vertexAttribs[1].format = VK_FORMAT_R32G32B32_SFLOAT;                   // 法向量输入属性的数据格式
//  vertexAttribs[1].offset = 12;                                           // 法向量输入属性的偏移量
  /// Sample5_3、Sample6_6 ***************************************** end

  /// Sample6_1、Sample6_7、Sample6_10 **************************** start
  vertexAttribs[1].binding = 0;
  vertexAttribs[1].location = 1;
  vertexAttribs[1].format = VK_FORMAT_R32G32_SFLOAT;
  vertexAttribs[1].offset = 12;
  /// Sample6_1、Sample6_7、Sample6_10 ****************************** end
}

/**
 * 创建管线
 */
void ShaderQueueSuit_Common::create_pipe_line(VkDevice &device, VkRenderPass &renderPass) {
  /// Sample4_1 ************************************************** start
//  VkDynamicState dynamicStateEnables[1];                                  // 动态状态启用标志数组
//  dynamicStateEnables[0] = VK_DYNAMIC_STATE_VIEWPORT;                     // 视口为动态设置
  /// Sample4_1 **************************************************** end

  /// Sample4_2、Sample4_14 ************************************** start
  VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];        // 动态状态启用标志
  memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);           // 设置所有标志为false
  /// Sample4_2、Sample4_14 **************************************** end

  /// Sample4_13 ************************************************* start
//  VkDynamicState dynamicStateEnables[1];                                  // 动态状态启用标志数组
//  dynamicStateEnables[0] = VK_DYNAMIC_STATE_DEPTH_BIAS;                   // 深度偏移为动态设置
  /// Sample4_13 *************************************************** end

  // 管线动态状态是指在程序运行过程中可以通过命令修改的一些参数，只有启用了某方面的动态状态才可以动态修改此方面的参数(如剪裁窗口、视口等)
  VkPipelineDynamicStateCreateInfo dynamicState = {};                     // 管线动态状态创建信息
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.pNext = nullptr;
  dynamicState.pDynamicStates = dynamicStateEnables;                      // 动态状态启用标志数组
//  dynamicState.dynamicStateCount = 1;                                     // Sample4_1、Sample4_13-启用的动态状态项数量
  dynamicState.dynamicStateCount = 0;                                     // Sample4_2、Sample4_14-启用的动态状态项数量(本案例没有这方面需要)

  VkPipelineVertexInputStateCreateInfo vi;                                // 管线顶点数据输入状态创建信息
  vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vi.pNext = nullptr;
  vi.flags = 0;
  vi.vertexBindingDescriptionCount = 1;                                   // 顶点输入绑定描述数量
  vi.pVertexBindingDescriptions = &vertexBinding;                         // 顶点输入绑定描述列表
  vi.vertexAttributeDescriptionCount = 2;                                 // 顶点输入属性描述数量
//  vi.vertexAttributeDescriptionCount = 1;                                 // Sample5_1、Sample6_8
  vi.pVertexAttributeDescriptions = vertexAttribs;                        // 顶点输入属性描述列表

  VkPipelineInputAssemblyStateCreateInfo ia;                              // 管线图元组装状态创建信息
  ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  ia.pNext = nullptr;
  ia.flags = 0;
  ia.primitiveRestartEnable = VK_FALSE;                                   // 关闭图元重启
  ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;                      // 采用三角形图元列表模式进行图元组装
//  ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;                       // Sample4_10、Sample4_16
//  ia.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;                         // Sample6_6、Sample6_8

  /// Sample4_7 ************************************************** start
//  VkPipelineInputAssemblyStateCreateInfo ia[topologyCount];               // 管线图元组装状态创建信息数组
//  for (int i = 0; i < topologyCount; ++i) {                               // 遍历数组中每个元素进行初始化
//    ia[i].sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//    ia[i].pNext = nullptr;
//    ia[i].flags = 0;
//    ia[i].primitiveRestartEnable = VK_FALSE;                              // 关闭图元重启
//    switch (i) {                                                          // 数组中的每个元素设置不同的图元组装方式
////      case 0:ia[i].topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;           // Sample4_7-点列表方式
//      case 0:ia[i].topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;       // Sample4_8-三点成面，点共用
//        break;
////      case 1:ia[i].topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;            // Sample4_7-线段列表方式
//      case 1:ia[i].topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;         // Sample4_8-三点成面，点共用
//        break;
//      case 2:ia[i].topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;           // 折线方式
//        break;
//      default:break;
//    }
//  }
  /// Sample4_7 **************************************************** end

  VkPipelineRasterizationStateCreateInfo rs;                              // 管线光栅化状态创建信息
  rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rs.pNext = nullptr;
  rs.flags = 0;
  rs.polygonMode = VK_POLYGON_MODE_FILL;                                  // 绘制方式为填充
  rs.cullMode = VK_CULL_MODE_NONE;                                        // 不使用背面剪裁
//  rs.cullMode = VK_CULL_MODE_BACK_BIT;                                    // Sample4_14、Sample6_9-开启背面剪裁
  rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;                         // 卷绕方向为逆时针
  rs.depthClampEnable = VK_TRUE;                                          // 深度截取
  rs.rasterizerDiscardEnable = VK_FALSE;                                  // 启用光栅化操作(若为TRUE则光栅化不产生任何片元)
  rs.depthBiasEnable = VK_FALSE;                                          // 不启用深度偏移
//  rs.depthBiasEnable = VK_TRUE;                                           // Sample4_13-启用深度偏移
  rs.depthBiasConstantFactor = 0;                                         // 深度偏移常量因子，启动深度偏移后，片元深度将加上此值
  rs.depthBiasClamp = 0;                                                  // 深度偏移值上下限(若为正作为上限，为负作为下限)
  rs.depthBiasSlopeFactor = 0;                                            // 深度偏移斜率因子，深度偏移计算中应用于片元斜率的标量因子
  rs.lineWidth = 1.0f;                                                    // 线宽度(仅在线绘制模式起作用)
//  rs.lineWidth = 10.0f;                                                   // Sample4_7-设置线宽时还需要wideLines特性支持和启用

  VkPipelineColorBlendAttachmentState att_state[1];                       // 管线颜色混合附件状态数组
  att_state[0].colorWriteMask = 0xf;                                      // 设置写入掩码
  att_state[0].blendEnable = VK_FALSE;                                    // 关闭混合
  att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;                            // 设置Alpha通道混合方式
  att_state[0].colorBlendOp = VK_BLEND_OP_ADD;                            // 设置RGB通道混合方式
  att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;                // 设置源颜色混合因子
  att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;                // 设置目标颜色混合因子
  att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;                // 设置源Alpha混合因子
  att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;                // 设置目标Alpha混合因子

  VkPipelineColorBlendStateCreateInfo cb;                                 // 管线的颜色混合状态创建信息
  cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  cb.pNext = nullptr;
  cb.flags = 0;
  cb.attachmentCount = 1;                                                 // 颜色混合附件数量
  cb.pAttachments = att_state;                                            // 颜色混合附件列表
  cb.logicOpEnable = VK_FALSE;                                            // 不启用逻辑操作
  cb.logicOp = VK_LOGIC_OP_NO_OP;                                         // 逻辑操作类型为无
  cb.blendConstants[0] = 1.0f;                                            // 混合常量R分量
  cb.blendConstants[1] = 1.0f;                                            // 混合常量G分量
  cb.blendConstants[2] = 1.0f;                                            // 混合常量B分量
  cb.blendConstants[3] = 1.0f;                                            // 混合常量A分量

  VkViewport viewports;                                                   // 视口信息
  viewports.minDepth = 0.0f;                                              // 视口最小深度
  viewports.maxDepth = 1.0f;                                              // 视口最大深度
  viewports.x = 0;                                                        // 视口x坐标
  viewports.y = 0;                                                        // 视口y坐标
  viewports.width = (float) MyVulkanManager::screenWidth;                 // 视口宽度
  viewports.height = (float) MyVulkanManager::screenHeight;               // 视口高度

  VkRect2D scissor;                                                       // 剪裁窗口信息
  scissor.extent.width = MyVulkanManager::screenWidth;                    // 剪裁窗口的宽度
  scissor.extent.height = MyVulkanManager::screenHeight;                  // 剪裁窗口的高度
  scissor.offset.x = 0;                                                   // 剪裁窗口的x坐标
  scissor.offset.y = 0;                                                   // 剪裁窗口的y坐标

  VkPipelineViewportStateCreateInfo vp = {};                              // 管线视口状态创建信息
  vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vp.pNext = nullptr;
  vp.flags = 0;
  vp.viewportCount = 1;                                                   // 视口的数量
  vp.scissorCount = 1;                                                    // 剪裁窗口的数量
  vp.pScissors = &scissor;                                                // 剪裁窗口信息列表
  vp.pViewports = &viewports;                                             // 视口信息列表

  VkPipelineDepthStencilStateCreateInfo ds;                               // 管线深度及模板状态创建信息
  ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  ds.pNext = nullptr;
  ds.flags = 0;
  ds.depthTestEnable = VK_TRUE;                                           // 开启深度测试
  ds.depthWriteEnable = VK_TRUE;                                          // 开启深度值写入
  ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;                        // 深度检测比较操作(表示深度测试在小于等于原有值的情况下通过)
  ds.depthBoundsTestEnable = VK_FALSE;                                    // 关闭深度边界测试
  ds.minDepthBounds = 0;                                                  // 最小深度边界
  ds.maxDepthBounds = 0;                                                  // 最大深度边界
  ds.stencilTestEnable = VK_FALSE;                                        // 关闭模板测试
  ds.back.failOp = VK_STENCIL_OP_KEEP;                                    // 未通过模板测试时的操作
  ds.back.passOp = VK_STENCIL_OP_KEEP;                                    // 模板测试、深度测试都通过时的操作
  ds.back.compareOp = VK_COMPARE_OP_ALWAYS;                               // 模板测试的比较操作
  ds.back.compareMask = 0;                                                // 模板测试比较掩码
  ds.back.reference = 0;                                                  // 模板测试参考值
  ds.back.depthFailOp = VK_STENCIL_OP_KEEP;                               // 未通过深度测试时的操作
  ds.back.writeMask = 0;                                                  // 写入掩码
  ds.front = ds.back;

  VkPipelineMultisampleStateCreateInfo ms;                                // 管线多重采样状态创建信息
  ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  ms.pNext = nullptr;
  ms.flags = 0;
  ms.pSampleMask = nullptr;                                               // 采样掩码
  ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;                        // 光栅化阶段采样数量
  ms.sampleShadingEnable = VK_FALSE;                                      // 关闭采样着色
  // 启用后在透明和不透明交界处的纹理像素会被进行多重采样，达到抗锯齿的效果，
  // 室外场景将大大受益于这种技术，通过透明纹理呈现的树叶、铁丝网等的边缘将会更加平滑
  ms.alphaToCoverageEnable = VK_FALSE;                                    // 不启用alphaToCoverage
  // 启用后所有片元的alpha值都会用1.0替代
  ms.alphaToOneEnable = VK_FALSE;                                         // 不启用alphaToOne
  ms.minSampleShading = 0.0;                                              // 最小采样着色

  VkGraphicsPipelineCreateInfo pipelineInfo;                              // 图形管线创建信息
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.pNext = nullptr;
  pipelineInfo.layout = pipelineLayout;                                   // 指定管线布局
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;                       // 基管线句柄
  pipelineInfo.basePipelineIndex = 0;                                     // 基管线索引
  // 该flags有3种选择：
  // VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT:表示创建管线时不启用最优化，创建管线的总时间可以缩短(由于没有对管线进行优化)
  // VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT:表示后面创建的管线可通过该选项创建的管线衍生，即该选项创建的管线作为后继创建管线的父管线
  // VK_PIPELINE_CREATE_DERIVATIVE_BIT:表示创建的管线作为前面创建管线的子管线，与VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT对应
  pipelineInfo.flags = 0;                                                 // 标志
  pipelineInfo.pVertexInputState = &vi;                                   // 管线的顶点数据输入状态信息
  pipelineInfo.pInputAssemblyState = &ia;                                 // Sample4_7-管线的图元组装状态信息
  pipelineInfo.pRasterizationState = &rs;                                 // 管线的光栅化状态信息
  pipelineInfo.pColorBlendState = &cb;                                    // 管线的颜色混合状态信息
  pipelineInfo.pTessellationState = nullptr;                              // 管线的曲面细分状态信息
  pipelineInfo.pMultisampleState = &ms;                                   // 管线的多重采样状态信息
  pipelineInfo.pDynamicState = &dynamicState;                             // 管线的动态状态信息
  pipelineInfo.pViewportState = &vp;                                      // 管线的视口状态信息
  pipelineInfo.pDepthStencilState = &ds;                                  // 管线的深度模板测试状态信息
  pipelineInfo.stageCount = 2;                                            // 管线的着色阶段数量
  pipelineInfo.pStages = shaderStages;                                    // 管线的着色阶段列表
  pipelineInfo.renderPass = renderPass;                                   // 管线的渲染通道
  pipelineInfo.subpass = 0;                                               // 设置管线执行对应的渲染子通道

  VkPipelineCacheCreateInfo pipelineCacheInfo;                            // 管线缓冲创建信息
  pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  pipelineCacheInfo.pNext = nullptr;
  pipelineCacheInfo.initialDataSize = 0;                                  // 初始数据尺寸
  pipelineCacheInfo.pInitialData = nullptr;                               // 指向管线缓冲初始化用预置内容数据首地址的指针
  pipelineCacheInfo.flags = 0;

  VkResult result = vk::vkCreatePipelineCache(device, &pipelineCacheInfo, nullptr, &pipelineCache);
  assert(result == VK_SUCCESS);                                           // 检查管线缓冲创建是否成功

  result = vk::vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineInfo, nullptr, &pipeline);
  assert(result == VK_SUCCESS);                                           // 检查管线创建是否成功
  /// Sample4_7 ************************************************** start
//  for (int i = 0; i < topologyCount; ++i) {                               // 为每种图元组装方式创建管线
//    pipelineInfo.pInputAssemblyState = &ia[i];                            // 指定管线的图元组装状态信息
//    result = vk::vkCreateGraphicsPipelines(                               // 创建管线
//        device, pipelineCache, 1, &pipelineInfo, nullptr, &pipeline[i]);
//    assert(result == VK_SUCCESS);                                         // 检查管线创建是否成功
//  }
  /// Sample4_7 **************************************************** end
}

/**
 * 销毁管线(和管线缓冲)
 */
void ShaderQueueSuit_Common::destroy_pipe_line(VkDevice &device) {
  vk::vkDestroyPipeline(device, pipeline, nullptr);
  /// Sample4_7 ************************************************** start
//  for (int i = 0; i < topologyCount; ++i) {
//    vk::vkDestroyPipeline(device, pipeline[i], nullptr);
//  }
  /// Sample4_7 **************************************************** end

  vk::vkDestroyPipelineCache(device, pipelineCache, nullptr);
}
