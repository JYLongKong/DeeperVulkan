#include "FileUtil.h"
#include <cassert>

AAssetManager *FileUtil::aam;

/**
 * 初始化AAssetManager对象
 */
void FileUtil::setAAssetManager(AAssetManager *aamIn) {
  aam = aamIn;
}

/**
 * 加载Assets文件夹下的指定文本性质文件内容作为字符串返回
 */
string FileUtil::loadAssetStr(string fname) {
  // 创建AAsset对象
  AAsset *asset = AAssetManager_open(aam, fname.c_str(), AASSET_MODE_UNKNOWN);
  // 获取AAsset对象的长度(以字节计)
  off_t fileSize = AAsset_getLength(asset);
  // 分配内存（+1是因为字符串最后需要\0）
  unsigned char *data = (unsigned char *) malloc(fileSize + 1);
  // 设置最后一个字节为字符串结束符'\0'
  data[fileSize] = '\0';
  // 读取文件内容
  int readBytesCount = AAsset_read(asset, (void *) data, fileSize);
  assert(readBytesCount == fileSize);
  // 产生结果字符串
  std::string resultStr((const char *) data);
  return resultStr;
}

/**
 * 加载Assets文件夹下的SPIR-V数据文件
 */
SpvData &FileUtil::loadSPV(string fname) {
  AAsset *asset = AAssetManager_open(aam, fname.c_str(), AASSET_MODE_STREAMING);
  assert(asset);
  size_t size = AAsset_getLength(asset);                                  // 获取SPIR-V数据文件的总字节数
  assert(size > 0);                                                       // 检查总字节数是否大于0
  SpvData spvData;                                                        // 构建SpvData结构体实例
  spvData.size = size;                                                    // 设置SPIR-V数据总字节数
  spvData.data = (uint32_t * )(malloc(size));                             // 分配相应字节数的内存
  AAsset_read(asset, spvData.data, size);                                 // 从文件中加载数据进入内存
  AAsset_close(asset);                                                    // 关闭AAsset对象
  return spvData;                                                         // 返回spvData结构体实例
}

/// Sample6_1 ************************************************** start
/**
 * 将字节序列转换为int值
 */
int fromBytesToInt(unsigned char *buff) {
  int k = 0;                                                              // 结果变量
  auto *temp = (unsigned char *) (&k);                                    // 将结果变量所占内存以字节序列模式访问
  temp[0] = buff[0];                                                      // 设置第1个字节的数据
  temp[1] = buff[1];                                                      // 设置第2个字节的数据
  temp[2] = buff[2];                                                      // 设置第3个字节的数据
  temp[3] = buff[3];                                                      // 设置第4个字节的数据
  return k;                                                               // 返回结果值
}

/**
 * 加载bntex纹理数据
 */
TexDataObject *FileUtil::loadCommonTexData(string fname) {
  AAsset *asset = AAssetManager_open(aam, fname.c_str(), AASSET_MODE_UNKNOWN); // 创建AAsset对象
  auto *buf = new unsigned char[4];                                       // 开辟长度为4字节的内存
  AAsset_read(asset, (void *) buf, 4);                              // 读取纹理宽度数据字节
  int width = fromBytesToInt(buf);                                        // 转换为int型数值
  AAsset_read(asset, (void *) buf, 4);                              // 读取纹理高度数据字节
  int height = fromBytesToInt(buf);                                       // 转换为int型数值
  auto *data = new unsigned char[width * height * 4];                     // 开辟纹理数据存储内存
  AAsset_read(asset, (void *) data, width * height * 4);            // 读取纹理数据
  auto *ctdo = new TexDataObject(width, height, data, width * height * 4); // 创建纹理数据对象

  return ctdo;
}
/// Sample6_1 **************************************************** end
