#include "LoadUtil.h"

#include <vector>

#include "FileUtil.h"

using namespace std;

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
    } else if (splitStrs[0] == "f") {                                     // 若此行为f开头则为面数据行
      int index[3];                                                       // 存放当前面三个顶点编号的数组
      string delimsF = "/";                                               // 用于切分每个顶点数据子串的分隔符

      splitStrsF.clear();
      splitString(splitStrs[1].c_str(), delimsF, splitStrsF);    // 切分第一个顶点的数据
      index[0] = parseInt(splitStrsF[0].c_str()) - 1;               // 获取当前面第一个顶点的编号
      alvResult.push_back(alv[3 * index[0]]);                             // 将第一个顶点的x,y,z坐标存入结果顶点坐标列表
      alvResult.push_back(alv[3 * index[0] + 1]);
      alvResult.push_back(alv[3 * index[0] + 2]);

      splitStrsF.clear();
      splitString(splitStrs[2].c_str(), delimsF, splitStrsF);    // 切分第二个顶点的数据
      index[1] = parseInt(splitStrsF[0].c_str()) - 1;               // 获取当前面第二个顶点的编号
      alvResult.push_back(alv[3 * index[1]]);                             // 将第二个顶点的x,y,z坐标存入结果顶点坐标列表
      alvResult.push_back(alv[3 * index[1] + 1]);
      alvResult.push_back(alv[3 * index[1] + 2]);

      splitStrsF.clear();
      splitString(splitStrs[3].c_str(), delimsF, splitStrsF);    // 切分第三个顶点的数据
      index[2] = parseInt(splitStrsF[0].c_str()) - 1;               // 获取当前面第三个顶点的编号
      alvResult.push_back(alv[3 * index[2]]);                             // 将第三个顶点的x,y,z坐标存入结果顶点坐标列表
      alvResult.push_back(alv[3 * index[2] + 1]);
      alvResult.push_back(alv[3 * index[2] + 2]);
    }
    splitStrs.clear();
  }

  int vCount = (int) alvResult.size() / 3;
  int dataByteCount = vCount * 3 * sizeof(float);
  float *vdataIn = new float[vCount * 3];
  int indexTemp = 0;
  for (int i = 0; i < vCount; i++) {
    vdataIn[indexTemp++] = alvResult[i * 3];
    vdataIn[indexTemp++] = alvResult[i * 3 + 1];
    vdataIn[indexTemp++] = alvResult[i * 3 + 2];
  }

  lo = new DrawableObjectCommon(vdataIn, dataByteCount, vCount, device, memoryProperties);
  return lo;
}
