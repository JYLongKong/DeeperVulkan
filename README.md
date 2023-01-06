# DeeperVulkan

Samples of Compute Graphics using Vulkan API for **Android**

## Prerequisites

You should use a device or an emulator that supports Vulkan, running Android 7.0 (Nougat), API level 24 or higher.

## Pictures

- **Sample5_5**

![](https://github.com/JYLongKong/DeeperVulkan/blob/main/screenshot/Sample5_5.gif)

- **Sample6_6**

![](https://github.com/JYLongKong/DeeperVulkan/blob/main/screenshot/Sample6_6.gif)

- **Sample7_5**

![](./screenshot/Sample7_5.jpg)

## How to read

- You can see [Releases](https://github.com/JYLongKong/DeeperVulkan/releases "releases") to get milestone samples.
- You can see [Tags](https://github.com/JYLongKong/DeeperVulkan/tags "Tags") to get every single feature(towards the Releases step by step).
- You can see Sample.apk in **./apk** directory in every sample series.
- You can see pictures in **./screenshot** directory for every release.

There is some **code comments** like below to point out features and difference in the sample project.
```
vertexBinding.stride = sizeof(float) * 8;                               // Sample7_4-point+texture+normal

  /// Sample7_4 *************************************************** start
  vertexAttribs[1].binding = 0;
  vertexAttribs[1].location = 1;
  vertexAttribs[1].format = VK_FORMAT_R32G32_SFLOAT;
  vertexAttribs[1].offset = 12;
  /// Sample7_4 ***************************************************** end
```

## Main process

| Step | Function | Description | 描述 |
| - | - | - |
| 1 | init_vulkan_instance() | Create vulkan instance | 创建Vulkan实例 |
| 2 | enumerate_vulkan_phy_devices() | Get physical device list |  获取物理设备列表 |
| 3 | create_vulkan_devices() | Create logical device | 创建逻辑设备 |
| 4 | create_vulkan_CommandBuffer() |  Create command buffer | 创建命令缓冲 |
| 5 | init_queue() | Get the queue which supports graphics | 获取设备中支持图形工作的队列 |
| 6 | create_vulkan_swapChain() | Create swap chain | 初始化交换链 |
| 7 | create_vulkan_DepthBuffer() | Create depth buffer | 创建深度缓冲 |
| 8 | create_render_pass() | Create render pass | 创建渲染通道 |
| 9 | create_frame_buffer() | Create frame buffer | 创建帧缓冲 |
| 10 | createDrawableObject() | Create drawable object | 创建绘制用的物体 |
| 11 | initPipeline() | Initialize render pipeline | 初始化渲染管线 |
| 12 | createFence() | Create fence | 创建栅栏 |
| 13 | initPresentInfo() | Initialize present information | 初始化呈现信息 |
| 14 | initMatrix() | Initialize transformation, camera and projection matrix  | 初始化基本变换矩阵、摄像机矩阵和投影矩阵 |
| 15 | drawObject() | Executes the drawing object  | 执行绘制 |
| 16 | destroyFence() | Destroy fence | 销毁栅栏 |
| 17 | destroyPipeline() | Destroy pipeline | 销毁管线 |
| 18 | destroyDrawableObject() | Destroy drawable object | 销毁绘制用物体 |
| 19 | destroy_frame_buffer() | Destroy frame buffer | 销毁帧缓冲 |
| 20 | destroy_render_pass() | Destroy render pass | 销毁渲染通道相关 |
| 21 | destroy_vulkan_DepthBuffer() | Destroy depth buffer | 销毁深度缓冲相关 |
| 22 | destroy_vulkan_swapChain() | Destroy swap chain | 销毁交换链相关 |
| 23 | destroy_vulkan_CommandBuffer() | Destroy command buffer | 销毁命令缓冲 |
| 24 | destroy_vulkan_devices() | Destroy logical device | 销毁逻辑设备 |
| 25 | destroy_vulkan_instance() | Destroy vulkan instance | 销毁Vulkan实例 |

## Samples

| Sample | Description | 描述 |
| - | - | - |
| Sample1_1 | First triangle | 第一个三角形 |
| Sample2_1 | Load SPIR-V shaders | 加载SPIR-V格式着色器 |
| Sample2_2 | Validate layer | 层级调试验证 |
| Sample4_1 | Movable triangle | 可移动的三角形 |
| Sample4_2 | Six-pointed star-orthogonal projection | 六角星-正交投影 |
| Sample4_3 | Six-pointed star-perspective projection | 六角星-透视投影 |
| Sample4_4 | Cube-translation transformation | 立方体-平移变换 |
| Sample4_5 | Cube-rotation transformation | 立方体-旋转变换 |
| Sample4_6 | Cube-scale transformation | 立方体-缩放变换 |
| Sample4_7 | Points and lines | 点与线段绘制方式 |
| Sample4_8 | Triangular strips and sectors | 三角条带与扇面绘制方式 |
| Sample4_9 | Draw belts by repeated vertex | 通过重复顶点绘制条带 |
| Sample4_10 | vkCmdDrawIndexed | 索引法绘制 |
| Sample4_11 | Different fov | 设置合理的视角 |
| Sample4_12 | Different NEAR cause artifact | 设置合理的投影参数 |
| Sample4_13 | SetDepthBias | 深度偏移 |
| Sample4_14 | Back cullMode | 卷绕和背面剪裁 |
| Sample4_15 | Indirectly draw by vertex | 顶点法间接绘制(基于Sample4_4) |
| Sample4_16 | Indirectly draw by index | 索引法间接绘制(基于Sample4_10) |
| Sample5_1 | Checkerboard ball | 黑白棋盘格的球 |
| Sample5_2 | Ambient light | 环境光照 |
| Sample5_3 | Diffused light | 散射光 |
| Sample5_4 | Specular light | 镜面光 |
| Sample5_5 | Ambient + Diffused + Specular | 环境光+散射光+镜面光 |
| Sample5_6 | Directional light | 定向光 |
| Sample5_7 | Surface normal vector | 立方体面法向量 |
| Sample5_8 | Point normal vector | 立方体点法向量 |
| Sample5_9 | Calculate light per fragment | 每片元计算光照 |
| Sample5_10 | Calculate light per vertex | 每顶点计算光照 |
| Sample6_1 | Texture triangle | 纹理三角形 |
| Sample6_2 | Flexible combination of color channels | 色彩通道的灵活组合 |
| Sample6_3 | Four ways to stretch texture | 四种纹理拉伸方式 |
| Sample6_4 | Two ways to sample texture | 两种纹理采样方式 |
| Sample6_5 | Mipmap texture | Mipmap纹理 |
| Sample6_6 | Multi-texture & procedural texture | 多重纹理与过程纹理 |
| Sample6_7 | Compressed texture | 压缩纹理 |
| Sample6_8 | Point sprites | 点精灵 |
| Sample6_9 | 3D texture | 3D纹理 |
| Sample6_10 | 2D-Array texture | 2D数组纹理 |
| Sample6_11 | Anisotropy | 各项异性过滤 |
| Sample7_1 | Load .obj file | 加载顶点与面数据的obj文件 |
| Sample7_2 | Calculate surface normal | 计算面法向量 |
| Sample7_3 | Calculate average normal | 计算平均法向量 |
| Sample7_4 | Load texture coordinates | 加载纹理坐标 |
| Sample7_5 | Load vertex normals | 加载顶点法向量 |

## Problem

If you encounter the following error when integrates Vulkan in Android NDK.

```
Build command failed.
Error while executing process D:\Android\SDK\cmake\3.18.1\bin\ninja.exe with arguments {-C E:\projects\Android\DeeperVulkan\app\.cxx\Debug\5l3u162n\arm64-v8a bn-vulkan-lib}
ninja: Entering directory `E:\projects\Android\DeeperVulkan\app\.cxx\Debug\5l3u162n\arm64-v8a'

ninja: error: 'D:/Android/SDK/ndk/21.4.7075529/sources/third_party/shaderc/libs/c++_static/arm64-v8a/libshaderc.a', needed by '../../../../build/intermediates/cxx/Debug/5l3u162n/obj/arm64-v8a/libbn-vulkan-lib.so', missing and no known rule to make it
```

Under the directory ```ndk/21.4.7075529/sources/third_party/shaderc``` run command:

```shell
$ ../../../ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk APP_ABI=all APP_PLATFORM=android-21 APP_STL=c++_static -j8 clean libshaderc_combined
```

If you encounter the following error when running command.

```
ndk/21.4.7075529/build/gmsl/__gmsl:512: *** non-numeric second argument to 'wordlist' function: 'android=24'.  Stop.
```

Then edit ```ndk/21.4.7075529/build/gmsl/__gmsl``` line 512 to:

```
int_encode = $(__gmsl_tr1)$(wordlist 1,$(words $1),$(__gmsl_input_int))
```

And try to run command again.

## Reference

**《Vulkan开发实战详解》**

![](https://img3.doubanio.com/view/subject/l/public/s33443430.jpg)
