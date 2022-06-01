#include "TextureManager.h"
#include "../bndev/mylog.h"
#include "HelpFunction.h"
#include "FileUtil.h"

std::vector<VkSampler> TextureManager::samplerList;
std::map<std::string, VkImage> TextureManager::textureImageList;
std::map<std::string, VkDeviceMemory> TextureManager::textureMemoryList;
std::map<std::string, VkImageView> TextureManager::viewTextureList;
std::map<std::string, VkDescriptorImageInfo> TextureManager::texImageInfoList;
std::map<std::string, int> TextureManager::imageSampler;                  // Sample6_3

//std::vector<std::string> TextureManager::texNames = {"texture/wall.bntex"}; // Sample6_1、Sample6_2
//std::vector<std::string> TextureManager::texNames =                       // Sample6_3
//    {"texture/robot0.bntex", "texture/robot1.bntex", "texture/robot2.bntex", "texture/robot3.bntex"};
//std::vector<std::string> TextureManager::texNames =                       // Sample6_4
//    {"texture/32Nearest.bntex", "texture/32Linear.bntex", "texture/256Nearest.bntex", "texture/256Linear.bntex"};
std::vector<std::string> TextureManager::texNames = {"texture/mipmap.bntex"}; // Sample6_5

void setImageLayout(VkCommandBuffer cmd,
                    VkImage image,
                    VkImageAspectFlags aspectMask,
                    VkImageLayout old_image_layout,
                    VkImageLayout new_image_layout) {
  VkImageMemoryBarrier image_memory_barrier = {};                         // 构建图像内存屏障结构体实例
  image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  image_memory_barrier.pNext = nullptr;
  image_memory_barrier.srcAccessMask = 0;                                 // 源访问掩码
  image_memory_barrier.dstAccessMask = 0;                                 // 目标访问掩码
  image_memory_barrier.oldLayout = old_image_layout;                      // 旧布局(屏障前)
  image_memory_barrier.newLayout = new_image_layout;                      // 新布局(屏障后)
  image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;     // 源队列家族索引
  image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;     // 目标队列家族索引
  image_memory_barrier.image = image;                                     // 对应的图像
  image_memory_barrier.subresourceRange.aspectMask = aspectMask;          // 使用方面
  image_memory_barrier.subresourceRange.baseMipLevel = 0;                 // 基础mipmap级别
  image_memory_barrier.subresourceRange.levelCount = 1;                   // mipmap级别的数量
  image_memory_barrier.subresourceRange.baseArrayLayer = 0;               // 基础数组层
  image_memory_barrier.subresourceRange.layerCount = 1;                   // 数组层的数量

  // 根据不同的新布局或旧布局预设值设置了源访问掩码或目标访问掩码
  if (old_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
    image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  }
  if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  }
  if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
    image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  }
  if (old_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  }
  if (old_image_layout == VK_IMAGE_LAYOUT_PREINITIALIZED) {
    image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
  }
  if (new_image_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
    image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  }
  if (new_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
    image_memory_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  }
  if (new_image_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    image_memory_barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  }

  VkPipelineStageFlags src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;    // 屏障前阶段(表示当管线刚开始执行的阶段)
  VkPipelineStageFlags dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;   // 屏障后阶段(表示当管线刚开始执行的阶段)
  // 放置图像内存屏障
  vk::vkCmdPipelineBarrier(
      cmd,                                                                // 实现指定内存屏障的命令缓冲
      src_stages,                                                         // 必须在指定内存屏障实现前执行完毕的管线阶段
      dest_stages,                                                        // 必须在指定内存屏障结束后才能开始执行的管线阶段
      0,                                                                  // 指定的内存屏障是否拥有屏幕空间位置
      0,                                                                  // 指定全局内存屏障的数量
      nullptr,                                                            // 指向全局内存屏障结构体实例列表的指针
      0,                                                                  // 指定缓冲内存屏障的数量
      nullptr,                                                            // 指向缓冲内存屏障结构体实例列表的指针
      1,                                                                  // 指定图像内存屏障的数量
      &image_memory_barrier                                               // 指向图像内存屏障结构体实例列表的指针
  );
}

void TextureManager::initSampler(VkDevice &device, VkPhysicalDevice &gpu) {
  VkSamplerCreateInfo samplerCreateInfo = {};                             // 构建采样器创建信息结构体实例
  samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;        // 结构体的类型

  /// 纹理采样方式
//  samplerCreateInfo.magFilter = VK_FILTER_LINEAR;                         // 放大时的纹理采样方式
  samplerCreateInfo.magFilter = VK_FILTER_NEAREST;                        // Sample6_5
  samplerCreateInfo.minFilter = VK_FILTER_NEAREST;                        // 缩小时的纹理采样方式
  /// Sample6_4 ************************************************** start
//  for (int i = 0; i < SAMPLER_COUNT; ++i) {                               // 循环设置不同的采样方式
//    if (i == 0) {
//      samplerCreateInfo.magFilter = VK_FILTER_NEAREST;                    // 放大时采用最近点采样方式
//      samplerCreateInfo.minFilter = VK_FILTER_NEAREST;                    // 缩小时采用最近点采样方式
//    } else if (i == 1) {
//      samplerCreateInfo.magFilter = VK_FILTER_LINEAR;                     // 放大时采用线性采样方式
//      samplerCreateInfo.minFilter = VK_FILTER_LINEAR;                     // 缩小时采用线性采样方式
//    }
  /// Sample6_4 **************************************************** end

  /// MipMap模式
//  samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;          // mipmap模式
  samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;           // Sample6_5

  /// 纹理拉伸方式
  samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE; // 纹理S轴的拉伸方式
  samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE; // 纹理T轴的拉伸方式
  samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE; // 纹理W轴的拉伸方式
  /// Sample6_3 ************************************************** start
//  for (int i = 0; i < SAMPLER_COUNT; ++i) {                               // 循环设置各种拉伸方式
//    if (i == 0) {                                                         // 设置为重复拉伸方式
//      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//      samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//      samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//    } else if (i == 1) {                                                  // 设置为截取拉伸方式
//      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//      samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//      samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//    } else if (i == 2) {                                                  // 设置为镜像重复拉伸方式
//      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
//      samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
//      samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
//    } else if (i == 3) {                                                  // 设置为边框拉伸方式
//      samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
//      samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
//      samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
//    }
  /// Sample6_3 **************************************************** end

  samplerCreateInfo.mipLodBias = 0.0;                                     // mipmap时的Lod调整值
  samplerCreateInfo.minLod = 0.0;                                         // 最小Lod值
//  samplerCreateInfo.maxLod = 0.0;                                         // 最大Lod值
  samplerCreateInfo.maxLod = 9.0;                                         // Sample6_5
  samplerCreateInfo.anisotropyEnable = VK_FALSE;                          // 是否启用各向异性过滤
  samplerCreateInfo.maxAnisotropy = 1;                                    // 各向异性最大过滤值
  samplerCreateInfo.compareEnable = VK_FALSE;                             // 是否开启比较功能
  samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;                      // 纹素数据比较操作
  samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;     // 要使用的预定义边框颜色

  for (int i = 0; i < SAMPLER_COUNT; ++i) {                               // 循环创建指定数量的采样器
    VkSampler samplerTexture;                                             // 声明采样器对象
    VkResult result = vk::vkCreateSampler(                                // 创建采样器
        device, &samplerCreateInfo, nullptr, &samplerTexture);
    samplerList.push_back(samplerTexture);                                // 将采样器加入列表
  }
}

void TextureManager::init_SPEC_2D_Textures(
    std::string texName,
    VkDevice &device,
    VkPhysicalDevice &gpu,
    VkPhysicalDeviceMemoryProperties &memoryroperties,
    VkCommandBuffer &cmdBuffer,
    VkQueue &queueGraphics,
    VkFormat format,
    TexDataObject *ctdo
) {
  VkFormatProperties formatProps;                                         // 指定格式纹理的格式属性
  vk::vkGetPhysicalDeviceFormatProperties(gpu, format, &formatProps);     // 获取指定格式纹理的格式属性
  bool needStaging = !(formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT); // 判断此格式纹理是否能使用线性瓦片纹理
  LOGI("TextureManager %s", (needStaging ? "不能使用线性瓦片纹理" : "能使用线性瓦片纹理"));

  if (needStaging) {
    // 不能使用线性瓦片纹理
    VkBuffer tempBuf;                                                     // 中转存储用的缓冲
    VkBufferCreateInfo buf_info = {};                                     // 构建缓冲创建信息结构体实例
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = nullptr;
    buf_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;                    // 缓冲的用途为传输源
    buf_info.size = ctdo->dataByteCount;                                  // 数据总字节数
    buf_info.queueFamilyIndexCount = 0;                                   // 队列家族数量
    buf_info.pQueueFamilyIndices = nullptr;                               // 队列家族索引列表
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;                     // 共享模式
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device, &buf_info, nullptr, &tempBuf); // 创建缓冲
    assert(result == VK_SUCCESS);

    VkMemoryRequirements mem_reqs;                                        // 缓冲的内存需求
    vk::vkGetBufferMemoryRequirements(device, tempBuf, &mem_reqs);        // 获取缓冲内存需求
    assert(ctdo->dataByteCount <= mem_reqs.size);                         // 检查内存需求获取是否正确

    VkMemoryAllocateInfo alloc_info = {};                                 // 构建内存分配信息结构体实例
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = nullptr;
    alloc_info.memoryTypeIndex = 0;                                       // 内存类型索引
    alloc_info.allocationSize = mem_reqs.size;                            // 内存总字节数
    VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |     // 需要的内存类型掩码
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag = memoryTypeFromProperties(                                 // 获取所需内存类型索引
        memoryroperties, mem_reqs.memoryTypeBits, requirements_mask, &alloc_info.memoryTypeIndex);
    if (flag) {
      LOGI("确定内存类型成功 类型索引为%d", alloc_info.memoryTypeIndex);
    } else {
      LOGE("确定内存类型失败!");
    }

    // 分配->映射->拷贝->解除映射->绑定
    VkDeviceMemory memTemp;                                               // 设备内存
    result = vk::vkAllocateMemory(device, &alloc_info, nullptr, &memTemp);// 分配设备内存
    assert(result == VK_SUCCESS);
    uint8_t *pData;                                                       // CPU访问时的辅助指针
    result = vk::vkMapMemory(device, memTemp, 0, mem_reqs.size, 0, (void **) &pData); // 将设备内存映射为CPU可访问
    assert(result == VK_SUCCESS);
    memcpy(pData, ctdo->data, ctdo->dataByteCount);                       // 将纹理数据拷贝进设备内存
    vk::vkUnmapMemory(device, memTemp);                                   // 解除内存映射
    result = vk::vkBindBufferMemory(device, tempBuf, memTemp, 0);         // 绑定内存与缓冲
    assert(result == VK_SUCCESS);

    VkImageCreateInfo image_create_info = {};                             // 构建图像创建信息结构体实例
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.pNext = nullptr;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;                       // 图像类型
    image_create_info.format = format;                                    // 图像像素格式
    image_create_info.extent.width = ctdo->width;                         // 图像宽度
    image_create_info.extent.height = ctdo->height;                       // 图像高度
    image_create_info.extent.depth = 1;                                   // 图像深度
    image_create_info.mipLevels = 1;                                      // 图像mipmap级数
    image_create_info.arrayLayers = 1;                                    // 图像数组层数量
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;                    // 采样模式
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;                   /// 采用最优瓦片组织方式
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;          // 初始布局
    image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT; // 图像用途
    image_create_info.queueFamilyIndexCount = 0;                          // 队列家族数量
    image_create_info.pQueueFamilyIndices = nullptr;                      // 队列家族索引列表
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;            // 共享模式
    image_create_info.flags = 0;                                          // 标志

    VkImage textureImage;                                                 // 纹理对应的图像
    result = vk::vkCreateImage(device, &image_create_info, nullptr, &textureImage); // 创建图像
    assert(result == VK_SUCCESS);
    textureImageList[texName] = textureImage;                             // 添加到纹理图像列表

    VkMemoryAllocateInfo mem_alloc = {};                                  // 构建内存分配信息结构体实例
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = nullptr;
    mem_alloc.allocationSize = 0;                                         // 内存总字节数
    mem_alloc.memoryTypeIndex = 0;                                        // 内存类型索引
    vk::vkGetImageMemoryRequirements(device, textureImage, &mem_reqs);    // 获取纹理图像内存需求
    mem_alloc.allocationSize = mem_reqs.size;                             // 实际分配的内存字节数
    flag = memoryTypeFromProperties(                                      // 获取内存类型索引
        memoryroperties, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);

    VkDeviceMemory textureMemory;                                         // 纹理图像对应设备内存
    result = vk::vkAllocateMemory(device, &mem_alloc, nullptr, &textureMemory); // 分配设备内存
    textureMemoryList[texName] = textureMemory;                           // 添加到纹理内存列表
    result = vk::vkBindImageMemory(device, textureImage, textureMemory, 0); // 将图像和设备内存绑定

    VkBufferImageCopy bufferCopyRegion = {};                              // 构建缓冲图像拷贝结构体实例
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // 使用方面
    bufferCopyRegion.imageSubresource.mipLevel = 0;                       // mipmap级别
    bufferCopyRegion.imageSubresource.baseArrayLayer = 0;                 // 基础数组层
    bufferCopyRegion.imageSubresource.layerCount = 1;                     // 数组层的数量
    bufferCopyRegion.imageExtent.width = ctdo->width;                     // 图像宽度
    bufferCopyRegion.imageExtent.height = ctdo->height;                   // 图像高度
    bufferCopyRegion.imageExtent.depth = 1;                               // 图像深度
    bufferCopyRegion.bufferOffset = 0;                                    // 偏移量

    VkCommandBufferBeginInfo cmd_buf_info = {};                           // 构建命令缓冲启动信息结构体实例
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.pNext = nullptr;
    cmd_buf_info.flags = 0;
    cmd_buf_info.pInheritanceInfo = nullptr;                              // 继承信息

    const VkCommandBuffer cmd_bufs[] = {cmdBuffer};                       // 命令缓冲数组
    VkSubmitInfo submit_info[1] = {};                                     // 提交信息数组
    submit_info[0].pNext = nullptr;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].waitSemaphoreCount = 0;                                // 等待的信号量数量
    submit_info[0].pWaitSemaphores = VK_NULL_HANDLE;                      // 等待的信号量列表
    submit_info[0].pWaitDstStageMask = VK_NULL_HANDLE;                    // 给定目标管线阶段
    submit_info[0].commandBufferCount = 1;                                // 命令缓冲的数量
    submit_info[0].pCommandBuffers = cmd_bufs;                            // 命令缓冲列表
    submit_info[0].signalSemaphoreCount = 0;                              // 任务完毕后设置的信号量数量
    submit_info[0].pSignalSemaphores = nullptr;                           // 任务完毕后设置的信号量数组

    VkFenceCreateInfo fenceInfo;                                          // 栅栏创建信息结构体实例
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = nullptr;
    fenceInfo.flags = 0;
    VkFence copyFence;                                                    // 拷贝任务用栅栏
    vk::vkCreateFence(device, &fenceInfo, nullptr, &copyFence);           // 创建栅栏

    vk::vkResetCommandBuffer(cmdBuffer, 0);                               // 清除命令缓冲
    result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);          // 启动命令缓冲(开始记录命令)
    setImageLayout(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT,  // 修改图像布局(为拷贝做准备)
                   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    vk::vkCmdCopyBufferToImage(                                           // 将缓冲中的数据拷贝到纹理图像中
        cmdBuffer, tempBuf, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);
    setImageLayout(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT,  // 修改图像布局(为纹理采样准备)
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    result = vk::vkEndCommandBuffer(cmdBuffer);                           // 结束命令缓冲(停止记录命令)

    result = vk::vkQueueSubmit(queueGraphics, 1, submit_info, copyFence); // 提交给队列执行
    do {                                                                  // 循环等待执行完毕
      result = vk::vkWaitForFences(device, 1, &copyFence, VK_TRUE, 100000000);
    } while (result == VK_TIMEOUT);
    vk::vkDestroyBuffer(device, tempBuf, nullptr);                        // 销毁中转缓冲
    vk::vkFreeMemory(device, memTemp, nullptr);                           // 释放中转缓冲的设备内存
    vk::vkDestroyFence(device, copyFence, nullptr);                       // 销毁拷贝任务用栅栏
  } else {
    // 能使用线性瓦片纹理
    VkImageCreateInfo image_create_info = {};                             // 构建图像创建信息结构体实例
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.pNext = nullptr;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;                       // 图像类型
    image_create_info.format = format;                                    // 图像像素格式
    image_create_info.extent.width = ctdo->width;
    image_create_info.extent.height = ctdo->height;
    image_create_info.extent.depth = 1;
    image_create_info.mipLevels = 1;                                      // 图像mipmap级数
    image_create_info.arrayLayers = 1;                                    // 图像数组层数量
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;                    // 采样模式
    image_create_info.tiling = VK_IMAGE_TILING_LINEAR;                    /// 采用线性瓦片组织方式
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;     // 初始布局
    image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT;                 // 图像用途
    image_create_info.queueFamilyIndexCount = 0;                          // 队列家族数量
    image_create_info.pQueueFamilyIndices = nullptr;                      // 队列家族索引列表
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;            // 共享模式
    image_create_info.flags = 0;                                          // 标志

    VkImage textureImage;                                                 // 纹理对应的图像
    VkResult result = vk::vkCreateImage(device, &image_create_info, nullptr, &textureImage); // 创建图像
    assert(result == VK_SUCCESS);
    textureImageList[texName] = textureImage;                             // 添加到纹理图像列表

    VkMemoryAllocateInfo mem_alloc = {};                                  // 构建内存分配信息结构体实例
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = nullptr;
    mem_alloc.allocationSize = 0;                                         // 内存字节数
    mem_alloc.memoryTypeIndex = 0;                                        // 内存类型索引
    VkMemoryRequirements mem_reqs;                                        // 纹理图像的内存需求
    vk::vkGetImageMemoryRequirements(device, textureImage, &mem_reqs);    // 获取纹理图像的内存需求
    mem_alloc.allocationSize = mem_reqs.size;                             // 实际分配的内存字节数
    bool flag = memoryTypeFromProperties(                                 // 获取内存类型索引
        memoryroperties, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &mem_alloc.memoryTypeIndex);

    VkDeviceMemory textureMemory;                                         // 创建设备内存实例
    result = vk::vkAllocateMemory(device, &mem_alloc, nullptr, &textureMemory); // 分配设备内存
    textureMemoryList[texName] = textureMemory;                           // 添加到纹理内存列表
    result = vk::vkBindImageMemory(device, textureImage, textureMemory, 0); // 绑定图像和内存
    uint8_t *pData;                                                       // CPU访问时的辅助指针
    vk::vkMapMemory(device, textureMemory, 0, mem_reqs.size, 0, (void **) (&pData)); // 映射内存为CPU可访问
    memcpy(pData, ctdo->data, mem_reqs.size);                             // 将纹理数据拷贝进设备内存
    vk::vkUnmapMemory(device, textureMemory);                             // 解除内存映射
  }

  VkImageViewCreateInfo view_info = {};                                   // 构建图像视图创建信息结构体实例
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.pNext = nullptr;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;                             // 图像视图的类型
  view_info.format = format;                                              // 图像视图的像素格式
  view_info.components.r = VK_COMPONENT_SWIZZLE_R;                        // 设置R通道调和
//  view_info.components.r = VK_COMPONENT_SWIZZLE_G;                        // Sample6_2-将纹理图中绿色通道的值映射到采样器的红色通道
  view_info.components.g = VK_COMPONENT_SWIZZLE_G;                        // 设置G通道调和
  view_info.components.b = VK_COMPONENT_SWIZZLE_B;                        // 设置B通道调和
  view_info.components.a = VK_COMPONENT_SWIZZLE_A;                        // 设置A通道调和
  view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;      // 图像视图使用方面
  view_info.subresourceRange.baseMipLevel = 0;                            // 基础Mipmap级别
  view_info.subresourceRange.levelCount = 1;                              // Mipmap级别的数量
  view_info.subresourceRange.baseArrayLayer = 0;                          // 基础数组层
  view_info.subresourceRange.layerCount = 1;                              // 数组层的数量
  view_info.image = textureImageList[texName];                            // 对应的图像

  VkImageView viewTexture;                                                // 纹理图像对应的图像视图
  VkResult result = vk::vkCreateImageView(device, &view_info, nullptr, &viewTexture);
  viewTextureList[texName] = viewTexture;                                 // 添加到图像视图列表

  VkDescriptorImageInfo texImageInfo;                                     // 构建图像描述信息结构体实例
  texImageInfo.imageView = viewTexture;                                   // 采用的图像视图
//  texImageInfo.sampler = samplerList[0];                                  // 采用的采样器
  texImageInfo.sampler = samplerList[imageSampler[texName]];              // Sample6_3、Sample6_4
  texImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;                     // 图像布局
  texImageInfoList[texName] = texImageInfo;                               // 添加到纹理图像描述信息列表

  delete ctdo;                                                            // 删除内存中的纹理数据
}

void TextureManager::init_SPEC_Textures_ForMipMap(
    std::string texName,
    VkDevice &device,
    VkPhysicalDevice &gpu,
    VkPhysicalDeviceMemoryProperties &memoryroperties,
    VkCommandBuffer &cmdBuffer,
    VkQueue &queueGraphics,
    VkFormat format,
    TexDataObject *ctdo,
    int levels
) {
  VkImageCreateInfo image_create_info = {};
  image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_create_info.pNext = nullptr;
  image_create_info.imageType = VK_IMAGE_TYPE_2D;
  image_create_info.format = format;
  image_create_info.extent.width = ctdo->width;
  image_create_info.extent.height = ctdo->height;
  image_create_info.extent.depth = 1;
  image_create_info.mipLevels = levels;
  image_create_info.arrayLayers = 1;
  image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
  image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  image_create_info.usage =
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  image_create_info.queueFamilyIndexCount = 0;
  image_create_info.pQueueFamilyIndices = nullptr;
  image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_create_info.flags = 0;

  VkImage textureImage;
  VkResult result = vk::vkCreateImage(device, &image_create_info, nullptr, &textureImage);
  assert(result == VK_SUCCESS);
  textureImageList[texName] = textureImage;

  VkMemoryAllocateInfo mem_alloc = {};
  mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  mem_alloc.pNext = nullptr;
  mem_alloc.allocationSize = 0;
  mem_alloc.memoryTypeIndex = 0;
  VkMemoryRequirements mem_reqs;
  vk::vkGetImageMemoryRequirements(device, textureImage, &mem_reqs);
  mem_alloc.allocationSize = mem_reqs.size;
  LOGI("IMG mem_reqs.size=%d", (int) (mem_reqs.size));
  bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, 0, &mem_alloc.memoryTypeIndex);
  assert(flag);

  VkDeviceMemory textureMemory;
  result = vk::vkAllocateMemory(device, &mem_alloc, nullptr, &(textureMemory));
  assert(result == VK_SUCCESS);
  textureMemoryList[texName] = textureMemory;
  result = vk::vkBindImageMemory(device, textureImage, textureMemory, 0);
  assert(result == VK_SUCCESS);

  /// 创建缓冲，将纹理数据首先搞进缓冲，然后传输进纹理
  VkBuffer stagingBuffer;
  VkBufferCreateInfo bufferCreateInfo = {};
  bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferCreateInfo.pNext = nullptr;
  bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  bufferCreateInfo.size = ctdo->dataByteCount;
  bufferCreateInfo.queueFamilyIndexCount = 0;
  bufferCreateInfo.pQueueFamilyIndices = nullptr;
  bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  bufferCreateInfo.flags = 0;
  result = vk::vkCreateBuffer(device, &bufferCreateInfo, nullptr, &stagingBuffer);
  assert(result == VK_SUCCESS);

  VkMemoryRequirements memReqs = {};
  vk::vkGetBufferMemoryRequirements(device, stagingBuffer, &memReqs);
  VkMemoryAllocateInfo memAllocInfo = {};
  memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  memAllocInfo.pNext = nullptr;
  memAllocInfo.memoryTypeIndex = 0;
  memAllocInfo.allocationSize = memReqs.size;
  memoryTypeFromProperties(memoryroperties, memReqs.memoryTypeBits,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           &memAllocInfo.memoryTypeIndex);

  VkDeviceMemory stagingMemory;
  result = vk::vkAllocateMemory(device, &memAllocInfo, nullptr, &stagingMemory);
  assert(result == VK_SUCCESS);
  result = vk::vkBindBufferMemory(device, stagingBuffer, stagingMemory, 0);
  uint8_t *pData;
  vk::vkMapMemory(device, stagingMemory, 0, memReqs.size, 0, (void **) (&pData));
  memcpy(pData, ctdo->data, memReqs.size);
  vk::vkUnmapMemory(device, stagingMemory);

  VkBufferImageCopy bufferCopyRegion = {};
  bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  bufferCopyRegion.imageSubresource.mipLevel = 0;
  bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
  bufferCopyRegion.imageSubresource.layerCount = 1;
  bufferCopyRegion.imageExtent.width = ctdo->width;
  bufferCopyRegion.imageExtent.height = ctdo->height;
  bufferCopyRegion.imageExtent.depth = 1;
  bufferCopyRegion.bufferOffset = 0;

  VkCommandBufferBeginInfo cmd_buf_info = {};
  cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  cmd_buf_info.pNext = nullptr;
  cmd_buf_info.flags = 0;
  cmd_buf_info.pInheritanceInfo = nullptr;

  const VkCommandBuffer cmd_bufs[] = {cmdBuffer};
  VkSubmitInfo submit_info[1] = {};
  submit_info[0].pNext = nullptr;
  submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info[0].waitSemaphoreCount = 0;
  submit_info[0].pWaitSemaphores = VK_NULL_HANDLE;
  submit_info[0].pWaitDstStageMask = VK_NULL_HANDLE;
  submit_info[0].commandBufferCount = 1;
  submit_info[0].pCommandBuffers = cmd_bufs;
  submit_info[0].signalSemaphoreCount = 0;
  submit_info[0].pSignalSemaphores = nullptr;

  VkFenceCreateInfo fenceInfo;
  VkFence copyFence;
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.pNext = nullptr;
  fenceInfo.flags = 0;
  vk::vkCreateFence(device, &fenceInfo, nullptr, &copyFence);

  vk::vkResetCommandBuffer(cmdBuffer, 0);
  result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);
  setImageLayout(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT,
                 VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  vk::vkCmdCopyBufferToImage(
      cmdBuffer, stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);
  setImageLayout(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT,
                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  for (int32_t i = 1; i < levels; ++i) {                                  // 遍历所有mipmap级数
    VkImageBlit imageBlit{};                                              // 创建图像blit实例
    imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;      // 使用方面
    imageBlit.srcSubresource.layerCount = 1;                              // 源资源的层数量
    imageBlit.srcSubresource.mipLevel = i - 1;                            // 源资源的mipmap级别
    imageBlit.srcOffsets[1].x = int32_t(ctdo->width >> (i - 1));          // 源资源的x偏移量
    imageBlit.srcOffsets[1].y = int32_t(ctdo->height >> (i - 1));         // 源资源的y偏移量
    imageBlit.srcOffsets[1].z = 1;                                        // 源资源的z偏移量
    imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;      // 使用方面
    imageBlit.dstSubresource.layerCount = 1;                              // 目标资源的层数量
    imageBlit.dstSubresource.mipLevel = i;                                // 目标资源的mipmap级别
    imageBlit.dstOffsets[1].x = int32_t(ctdo->width >> i);                // 目标资源的x偏移量
    imageBlit.dstOffsets[1].y = int32_t(ctdo->height >> i);               // 目标资源的y偏移量
    imageBlit.dstOffsets[1].z = 1;                                        // 目标资源的z偏移量
    vk::vkCmdBlitImage(                                                   // 从源资源生成目标资源(即通过上一层纹理数据生成下一层纹理数据)
        cmdBuffer,                                                        // 命令缓冲
        textureImage,                                                     // 源图像
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,                             // 源图像瓦片组织方式
        textureImage,                                                     // 目标图像
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,                             // 目标图像瓦片组织方式
        1,                                                                // 图像blit实例数量
        &imageBlit,                                                       // 图像blit实例列表
        VK_FILTER_LINEAR                                                  // 要应用的纹理采样过滤器
    );
  }

  result = vk::vkEndCommandBuffer(cmdBuffer);                             // 结束命令缓冲(停止记录命令)
  result = vk::vkQueueSubmit(queueGraphics, 1, submit_info, copyFence);   // 提交给队列执行
  do {
    result = vk::vkWaitForFences(device, 1, &copyFence, VK_TRUE, 100000000);
  } while (result == VK_TIMEOUT);
  vk::vkDestroyBuffer(device, stagingBuffer, nullptr);
  vk::vkFreeMemory(device, stagingMemory, nullptr);
  vk::vkDestroyFence(device, copyFence, nullptr);

  VkImageViewCreateInfo view_info = {};
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.pNext = nullptr;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  view_info.format = VK_FORMAT_R8G8B8A8_UNORM;
  view_info.components.r = VK_COMPONENT_SWIZZLE_R;
  view_info.components.g = VK_COMPONENT_SWIZZLE_G;
  view_info.components.b = VK_COMPONENT_SWIZZLE_B;
  view_info.components.a = VK_COMPONENT_SWIZZLE_A;
  view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  view_info.subresourceRange.baseMipLevel = 0;
  view_info.subresourceRange.levelCount = levels;
  view_info.subresourceRange.baseArrayLayer = 0;
  view_info.subresourceRange.layerCount = 1;
  view_info.image = textureImageList[texName];

  VkImageView viewTexture;
  result = vk::vkCreateImageView(device, &view_info, nullptr, &viewTexture);
  viewTextureList[texName] = viewTexture;

  VkDescriptorImageInfo texImageInfo;
  texImageInfo.imageView = viewTexture;
  texImageInfo.sampler = samplerList[0];
  texImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
  texImageInfoList[texName] = texImageInfo;

  delete ctdo;
}

void TextureManager::initTextures(VkDevice &device,
                                  VkPhysicalDevice &gpu,
                                  VkPhysicalDeviceMemoryProperties &memoryroperties,
                                  VkCommandBuffer &cmdBuffer,
                                  VkQueue &queueGraphics) {
  initSampler(device, gpu);                                         // 初始化采样器

  /// Sample6_5 ************************************************** start
  VkFormatProperties formatProps;                                         // 指定格式纹理的格式属性
  vk::vkGetPhysicalDeviceFormatProperties(gpu, VK_FORMAT_R8G8B8A8_UNORM, &formatProps); // 获取指定格式纹理的格式属性
  assert(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT); // 检查是否支持生成mipmap所需的BLIT类型的源和目标
  assert(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);
  /// Sample6_5 **************************************************** end

  for (int i = 0; i < texNames.size(); ++i) {                             // 遍历纹理文件名称列表
//    imageSampler[texNames[i]] = i;                                        // Sample6_3-设置对应纹理的采样器索引
//    imageSampler[texNames[i]] = i % 2;                                    // Sample6_4
    TexDataObject *ctdo = FileUtil::loadCommonTexData(texNames[i]); // 加载纹理文件数据
    int levels = floor(log2(max(ctdo->width, ctdo->height))) + 1;         // Sample6_5-计算mipmap层次数
    LOGI("%s: width=%d height=%d lod=%d", texNames[i].c_str(), ctdo->width, ctdo->height, levels); // 打印纹理数据信息
//    init_SPEC_2D_Textures(                                                // 加载2D纹理
//        texNames[i], device, gpu, memoryroperties, cmdBuffer, queueGraphics, VK_FORMAT_R8G8B8A8_UNORM, ctdo);
    init_SPEC_Textures_ForMipMap(                                         // Sample6_5-加载2D纹理并生成MipMap
        texNames[i], device, gpu, memoryroperties, cmdBuffer, queueGraphics, VK_FORMAT_R8G8B8A8_UNORM, ctdo, levels);
  }
}

void TextureManager::destroyTextures(VkDevice &device) {
  for (int i = 0; i < SAMPLER_COUNT; ++i) {                               // 遍历所有采样器
    vk::vkDestroySampler(device, samplerList[i], nullptr);                // 销毁采样器
  }
  for (int i = 0; i < texNames.size(); ++i) {                             // 遍历所有纹理
    vk::vkDestroyImageView(device, viewTextureList[texNames[i]], nullptr); // 销毁图像视图
    vk::vkDestroyImage(device, textureImageList[texNames[i]], nullptr);   // 销毁图像
    vk::vkFreeMemory(device, textureMemoryList[texNames[i]], nullptr);    // 释放设备内存
  }
}

int TextureManager::getVkDescriptorSetIndex(std::string texName) {
  int result = -1;
  for (int i = 0; i < texNames.size(); ++i) {                             // 遍历所有纹理
    if (texNames[i].compare(texName.c_str()) == 0) {                      // 判断名称是否相同
      result = i;                                                         // 以当前索引值为结果
      break;
    }
  }
  assert(result != -1);
  return result;
}
