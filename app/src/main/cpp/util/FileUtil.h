#ifndef __FileUtil_H__
#define __FileUtil_H__

#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include <string>

using namespace std;

typedef struct SpvDataStruct {  // 存储SPIR-V数据的结构体
  int size;                     // SPIR-V数据总字节数
  uint32_t *data;               // 指向SPIR-V数据内存块首地址的指针
} SpvData;

class FileUtil {
 public:
  static AAssetManager *aam;                          // 指向AAssetManager对象的指针
  static void setAAssetManager(AAssetManager *aamIn); // 初始化AAssetManager对象
  static string loadAssetStr(string fname);           // 加载Assets文件夹下的指定文本性质文件内容作为字符串返回
  static SpvData &loadSPV(string fname);              // 加载Assets文件夹下的SPIR-V数据
};

#endif