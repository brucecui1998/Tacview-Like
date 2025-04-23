#version 330 core
layout (location = 0) in vec3 position;  // 位置
layout (location = 1) in vec3 color;     // 颜色

out vec3 fragPosition;  // 输出顶点位置给片段着色器

void main()
{
    gl_Position = vec4(position, 1.0f);  // 顶点位置传递给 gl_Position
    fragPosition = position;             // 输出顶点位置到片段着色器
}
