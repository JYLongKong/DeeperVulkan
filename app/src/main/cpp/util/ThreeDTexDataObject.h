#ifndef DEEPERVULKAN_THREEDTEXDATAOBJECT_H_
#define DEEPERVULKAN_THREEDTEXDATAOBJECT_H_

class ThreeDTexDataObject {
 public:
  int width;
  int height;
  int depth;
  unsigned char *data;
  int dataByteCount;

  ThreeDTexDataObject(int width, int height, int depth, unsigned char *data);
  ~ThreeDTexDataObject();
};

#endif //DEEPERVULKAN_THREEDTEXDATAOBJECT_H_
