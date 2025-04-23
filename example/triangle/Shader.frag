#version 330 core
in vec3 fragPosition;  // 接收顶点着色器传来的顶点位置

out vec4 color;  // 片段着色器输出的颜色

void main()
{
    color = vec4(fragPosition, 1.0f);  // 将顶点位置作为颜色输出，R、G、B分别为X、Y、Z坐标
}
