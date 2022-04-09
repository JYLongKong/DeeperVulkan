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
  size_t size = AAsset_getLength(asset);      // 获取SPIR-V数据文件的总字节数
  assert(size > 0);                           // 检查总字节数是否大于0
  SpvData spvData;                            // 构建SpvData结构体实例
  spvData.size = size;                        // 设置SPIR-V数据总字节数
  spvData.data = (uint32_t *) (malloc(size)); // 分配相应字节数的内存
  AAsset_read(asset, spvData.data, size);     // 从文件中加载数据进入内存
  AAsset_close(asset);                        // 关闭AAsset对象
  return spvData;                             // 返回spvData结构体实例
}
