#ifndef DEEPERVULKAN_TEXDATAOBJECT_H_
#define DEEPERVULKAN_TEXDATAOBJECT_H_

class TexDataObject {
 public:
  int width;            // 纹理宽度
  int height;           // 纹理高度
  int dataByteCount;    // 纹理的数据总字节数
  unsigned char *data;  // 指向纹理数据存储内存首地址的指针

  TexDataObject(int width, int height, unsigned char *data, int dataByteCount);
  ~TexDataObject();
};

#endif // DEEPERVULKAN_TEXDATAOBJECT_H_
