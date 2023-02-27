#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) out vec4 outColor;
layout (location = 0) in vec4 inLightQD;
layout (location = 1) in vec4 inLightQDBack;

void main() {
    vec4 finalColor = vec4(0.9, 0.9, 0.9, 1.0);
    if (gl_FrontFacing) {   // 若当前片元是正面
        outColor = inLightQD * finalColor;
    } else {
        outColor = inLightQDBack * finalColor;
    }
}
