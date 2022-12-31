#include "LoadUtil.h"

#include <vector>
#include <cmath>
#include <map>
#include <set>

#include "FileUtil.h"
#include "Normal.h"

using namespace std;

float *LoadUtil::vectorNormal(float *vector) {
  float module = (float) sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
  return new float[3]{vector[0] / module, vector[1] / module, vector[2] / module};
}

/**
 * 求两个向量叉积
 */
float *getCrossProduct(float x1, float y1, float z1, float x2, float y2, float z2) {
  float A = y1 * z2 - y2 * z1;
  float B = z1 * x2 - z2 * x1;
  float C = x1 * y2 - x2 * y1;
  return new float[3]{A, B, C};
}

size_t splitString(const string &strSrc, const string &strDelims, vector<string> &strDest) {
  string delims = strDelims;
  string STR;
  if (delims.empty()) delims = " **";
  string::size_type pos = 0;
  string::size_type LEN = strSrc.size();
  while (pos < LEN) {
    STR = "";
    while ((delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN)) {
      ++pos;
    }
    if (pos == LEN) {
      return strDest.size();
    }
    while ((delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN)) {
      STR += strSrc[pos++];
    }
    if (!STR.empty()) {
      strDest.push_back(STR);
    }
  }
  return strDest.size();
}

bool tryParseDouble(const char *s, const char *s_end, double *result) {
  if (s >= s_end) {
    return false;
  }
  double mantissa = 0.0;
  int exponent = 0;
  char sign = '+';
  char exp_sign = '+';
  char const *curr = s;
  int read = 0;
  bool end_not_reached = false;
  if (*curr == '+' || *curr == '-') {
    sign = *curr;
    curr++;
  } else if (isdigit(*curr)) { /* Pass through. */ }
  else {
    goto fail;
  }
  while ((end_not_reached = (curr != s_end)) && isdigit(*curr)) {
    mantissa *= 10;
    mantissa += static_cast<int>(*curr - 0x30);
    curr++;
    read++;
  }
  if (read == 0)
    goto fail;
  if (!end_not_reached)
    goto assemble;
  if (*curr == '.') {
    curr++;
    read = 1;
    while ((end_not_reached = (curr != s_end)) && isdigit(*curr)) {
      mantissa += static_cast<int>(*curr - 0x30) * pow(10.0, -read);
      read++;
      curr++;
    }
  } else if (*curr == 'e' || *curr == 'E') {}
  else {
    goto assemble;
  }
  if (!end_not_reached)
    goto assemble;
  if (*curr == 'e' || *curr == 'E') {
    curr++;
    if ((end_not_reached = (curr != s_end)) && (*curr == '+' || *curr == '-')) {
      exp_sign = *curr;
      curr++;
    } else if (isdigit(*curr)) { /* Pass through. */ }
    else {
      goto fail;
    }
    read = 0;
    while ((end_not_reached = (curr != s_end)) && isdigit(*curr)) {
      exponent *= 10;
      exponent += static_cast<int>(*curr - 0x30);
      curr++;
      read++;
    }
    exponent *= (exp_sign == '+' ? 1 : -1);
    if (read == 0)
      goto fail;
  }
  assemble:
  *result = (sign == '+' ? 1 : -1) * ldexp(mantissa * pow(5.0, exponent), exponent);
  return true;
  fail:
  return false;
}

float parseFloat(const char *token) {
  token += strspn(token, " \t");
#ifdef TINY_OBJ_LOADER_OLD_FLOAT_PARSER
  float f = atof(token);
#else
  const char *end = token + strcspn(token, " \t\r");
  double val = 0.0;
  tryParseDouble(token, end, &val);
  float f = static_cast<float>(val);
#endif
  return f;
}

int parseInt(const char *token) {
  token += strspn(token, " \t");
  int i = atoi(token);
  return i;
}

DrawableObjectCommon *LoadUtil::loadFromFile(
    const std::string &fname,
    VkDevice &device,
    VkPhysicalDeviceMemoryProperties &memoryProperties
) {
  DrawableObjectCommon *lo;
  vector<float> alv;                                                      // 存放原始顶点坐标数据
  vector<float> alvResult;                                                // 存放结果顶点坐标数据
//  vector<float> alnResult;                                                // Sample7_2-存放结果法向量数据
  vector<int> alFaceIndex;                                                // Sample7_3-存放三角形面顶点编号
  map<int, set<Normal *>> hmn;                                            // Sample7_3-存放各顶点法向量
  vector<float> alt;                                                      // Sample7_4-存放原始纹理坐标数据
  vector<float> altResult;                                                // Sample7_4-存放结果纹理坐标数据

  string resultStr = FileUtil::loadAssetStr(fname);                 // 将obj文件内容加载为字符串
  vector<string> lines;                                                   // 存放obj文件各行字符串的列表
  splitString(resultStr, "\n", lines);                        // 用换行符"\n"切分obj文件内容
  vector<string> splitStrs;                                               // 存放一行内容切分后结果
  vector<string> splitStrsF;                                              // 存放一个顶点数据切分后结果
  string tempContents;                                                    // 声明缓存单行内容的辅助字符串
  for (int i = 0; i < lines.size(); i++) {                                // 遍历obj文件中每行的字符串
    tempContents = lines[i];
    if (tempContents.compare("") == 0) { continue; }                    // 当前行没有内容则跳过
    string delims = "[ ]+";                                               // 用于切分每行内容的分隔符字符串
    splitStrs.clear();
    splitString(tempContents, delims, splitStrs);                      // 用空格符切分每行内容
    if (splitStrs[0] == "v") {                                            // 若此行为v开头则为顶点坐标行
      alv.push_back(parseFloat(splitStrs[1].c_str()));              // 将顶点坐标存入原始顶点坐标列表
      alv.push_back(parseFloat(splitStrs[2].c_str()));
      alv.push_back(parseFloat(splitStrs[3].c_str()));
    } else if (splitStrs[0] == "vt") {                                    // Sample7_4-若此行为vt开头则为纹理坐标行
      alt.push_back(parseFloat(splitStrs[1].c_str()));              // Sample7_4-将纹理S坐标存入原始纹理坐标列表
      alt.push_back(1 - parseFloat(splitStrs[2].c_str()));          // Sample7_4-将纹理T坐标存入原始纹理坐标列表
    } else if (splitStrs[0] == "f") {                                     // 若此行为f开头则为面数据行
      int index[3];                                                       // 存放当前面三个顶点编号的数组
      string delimsF = "/";                                               // 用于切分每个顶点数据子串的分隔符

      splitStrsF.clear();
      splitString(splitStrs[1].c_str(), delimsF, splitStrsF);    // 切分第一个顶点的数据
      index[0] = parseInt(splitStrsF[0].c_str()) - 1;               // 获取当前面第一个顶点的编号
      float x0 = alv[3 * index[0]];
      float y0 = alv[3 * index[0] + 1];
      float z0 = alv[3 * index[0] + 2];
      alvResult.push_back(x0);                                             // 将第一个顶点的x,y,z坐标存入结果顶点坐标列表
      alvResult.push_back(y0);
      alvResult.push_back(z0);
      int indexTex = parseInt(splitStrsF[1].c_str()) - 1;           // Sample7_4-获取三角形面第1个顶点的纹理坐标编号
      altResult.push_back(alt[indexTex * 2]);                             // Sample7_4-将第1个顶点的纹理S坐标存入结果纹理坐标列表
      altResult.push_back(alt[indexTex * 2 + 1]);                         // Sample7_4-将第1个顶点的纹理T坐标存入结果纹理坐标列表

      splitStrsF.clear();
      splitString(splitStrs[2].c_str(), delimsF, splitStrsF);    // 切分第二个顶点的数据
      index[1] = parseInt(splitStrsF[0].c_str()) - 1;               // 获取当前面第二个顶点的编号
      float x1 = alv[3 * index[1]];
      float y1 = alv[3 * index[1] + 1];
      float z1 = alv[3 * index[1] + 2];
      alvResult.push_back(x1);                                            // 将第二个顶点的x,y,z坐标存入结果顶点坐标列表
      alvResult.push_back(y1);
      alvResult.push_back(z1);
      indexTex = parseInt(splitStrsF[1].c_str()) - 1;               // Sample7_4-获取三角形面第2个顶点的纹理坐标编号
      altResult.push_back(alt[indexTex * 2]);                             // Sample7_4-将第2个顶点的纹理S坐标存入结果纹理坐标列表
      altResult.push_back(alt[indexTex * 2 + 1]);                         // Sample7_4-将第2个顶点的纹理T坐标存入结果纹理坐标列表

      splitStrsF.clear();
      splitString(splitStrs[3].c_str(), delimsF, splitStrsF);    // 切分第三个顶点的数据
      index[2] = parseInt(splitStrsF[0].c_str()) - 1;               // 获取当前面第三个顶点的编号
      float x2 = alv[3 * index[2]];
      float y2 = alv[3 * index[2] + 1];
      float z2 = alv[3 * index[2] + 2];
      alvResult.push_back(x2);                                            // 将第三个顶点的x,y,z坐标存入结果顶点坐标列表
      alvResult.push_back(y2);
      alvResult.push_back(z2);
      indexTex = parseInt(splitStrsF[1].c_str()) - 1;               // Sample7_4-获取三角形面第3个顶点的纹理坐标编号
      altResult.push_back(alt[indexTex * 2]);                             // Sample7_4-将第3个顶点的纹理S坐标存入结果纹理坐标列表
      altResult.push_back(alt[indexTex * 2 + 1]);                         // Sample7_4-将第3个顶点的纹理T坐标存入结果纹理坐标列表


      /// Sample7_2-法向量
      float vxa = x1 - x0;                                                // 求三角形中第一个点到第二个点的向量
      float vya = y1 - y0;
      float vza = z1 - z0;
      float vxb = x2 - x0;                                                // 求三角形中第一个点到第三个点的向量
      float vyb = y2 - y0;
      float vzb = z2 - z0;
      float *vNormal = vectorNormal(getCrossProduct(vxa, vya, vza, vxb, vyb, vzb)); // 通过计算两个向量的叉积计算出此三角形面的法向量
//      for (int i = 0; i < 3; ++i) {                                       // 将计算出的法向量分量添加到结果法向量列表中
//        alnResult.push_back(vNormal[0]);
//        alnResult.push_back(vNormal[1]);
//        alnResult.push_back(vNormal[2]);
//      }

      /// Sample7_3-平均法向量
      alFaceIndex.push_back(index[0]);                                    // 记录三角形面三个顶点的编号
      alFaceIndex.push_back(index[1]);
      alFaceIndex.push_back(index[2]);
      for (int tempIndex: index) {                                        // 将此三角形面的法向量记录到此面3个顶点各自的法向量集合中
        set<Normal *> setN = hmn[tempIndex];                              // 由顶点编号获取对应的法向量集合
        Normal *normal = new Normal(vNormal[0], vNormal[1], vNormal[2]); // 创建法向量对象
        if (!Normal::exist(normal, setN)) {                            // 判断当前法向量是否不在当前点的法向量集合中
          setN.insert(normal);                                            // 若不在则将该法向量添加到当前点的法向量集合中
        }
        hmn[tempIndex] = setN;                                            // 更新map中当前点的法向量集合
      }

    }
    splitStrs.clear();
  }

  int vCount = (int) alvResult.size() / 3;
//  int dataByteCount = vCount * 3 * sizeof(float);
//  float *vdataIn = new float[vCount * 3];
//  int dataByteCount = vCount * 6 * sizeof(float);                         // Sample7_2
//  float *vdataIn = new float[vCount * 6];                                 // Sample7_2
  int dataByteCount = vCount * 8 * sizeof(float);                         // Sample7_4
  float *vdataIn = new float[vCount * 8];                                 // Sample7_4
  set<Normal *> setNTemp;                                                 // Sample7_3-存放一个顶点法向量集合的辅助变量
  float *nTemp;                                                           // Sample7_3-指向存放向量三分量数据数组的指针
  int indexTemp = 0;
  for (int i = 0; i < vCount; i++) {
    vdataIn[indexTemp++] = alvResult[i * 3];
    vdataIn[indexTemp++] = alvResult[i * 3 + 1];
    vdataIn[indexTemp++] = alvResult[i * 3 + 2];

    /// Sample7_2
//    vdataIn[indexTemp++] = alnResult[i * 3];
//    vdataIn[indexTemp++] = alnResult[i * 3 + 1];
//    vdataIn[indexTemp++] = alnResult[i * 3 + 2];

    /// Sample7_4-将纹理ST坐标转存到顶点数据数组中
    vdataIn[indexTemp++] = altResult[i * 2];
    vdataIn[indexTemp++] = altResult[i * 2 + 1];

    /// Sample7_3
    setNTemp = (hmn[alFaceIndex.at(i)]);                                  // 获取当前顶点的法向量集合
    nTemp = Normal::getAverage(setNTemp);                              // 求出此顶点的平均法向量
    vdataIn[indexTemp++] = nTemp[0];                                      // 将平均法向量的三个分量转存到顶点数据数组中
    vdataIn[indexTemp++] = nTemp[1];
    vdataIn[indexTemp++] = nTemp[2];
  }

  lo = new DrawableObjectCommon(vdataIn, dataByteCount, vCount, device, memoryProperties);
  return lo;
}
