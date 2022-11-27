#ifndef DEEPERVULKAN_TEXARRAYDATAOBJECT_H_
#define DEEPERVULKAN_TEXARRAYDATAOBJECT_H_

class TexArrayDataObject {
 public:
  int width;  // 纹理宽度
  int height; // 纹理高度
  int length; // 纹理数组长度
  unsigned char *data;  // 指向纹理数据存储内存首地址的指针
  int dataByteCount;    // 纹理的数据总字节数
  TexArrayDataObject(int width, int height, int length, unsigned char *data);
  ~TexArrayDataObject();
};

#endif //DEEPERVULKAN_TEXARRAYDATAOBJECT_H_
