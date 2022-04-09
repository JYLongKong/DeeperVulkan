#include "ShaderCompileUtil.h"
#include "mylog.h"

//服务于内部使用的类型映射方法
struct shader_type_mapping {
  VkShaderStageFlagBits vkshader_type;
  shaderc_shader_kind shaderc_type;
};

//服务于内部使用的类型映射方法
static const shader_type_mapping shader_map_table[] =
    {
        {
            VK_SHADER_STAGE_VERTEX_BIT,
            shaderc_glsl_vertex_shader
        },
        {
            VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
            shaderc_glsl_tess_control_shader
        },
        {
            VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
            shaderc_glsl_tess_evaluation_shader
        },
        {
            VK_SHADER_STAGE_GEOMETRY_BIT,
            shaderc_glsl_geometry_shader},
        {
            VK_SHADER_STAGE_FRAGMENT_BIT,
            shaderc_glsl_fragment_shader
        },
        {
            VK_SHADER_STAGE_COMPUTE_BIT,
            shaderc_glsl_compute_shader
        },
    };

//内部使用的类型映射方法
shaderc_shader_kind MapShadercType(VkShaderStageFlagBits vkShader) {
  for (auto shader : shader_map_table) {
    if (shader.vkshader_type == vkShader) {
      return shader.shaderc_type;
    }
  }
  return shaderc_glsl_infer_from_source;
}

bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader,
               std::vector<unsigned int> &spirv) {
  shaderc::Compiler compiler;
  shaderc::SpvCompilationResult module =
      compiler.CompileGlslToSpv(pshader, strlen(pshader),
                                MapShadercType(shader_type),
                                "shader");
  if (module.GetCompilationStatus() !=
      shaderc_compilation_status_success) {
    LOGE("Error: Id=%d, Msg=%s",
         module.GetCompilationStatus(),
         module.GetErrorMessage().c_str());
    return false;
  }
  spirv.assign(module.cbegin(), module.cend());
  return true;
}