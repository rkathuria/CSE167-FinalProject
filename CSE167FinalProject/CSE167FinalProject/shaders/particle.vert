//#version 330 core
//layout (location = 0) in vec4 aPos;
//
////out vec3 ourColor;
//out vec2 TexCoord;
//
//void main()
//{
//    gl_Position = vec4(aPos.xy, 0.0, 1.0);
////    ourColor = vec3(1.0f);
//    TexCoord = aPos.zw;
//}


#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoord;
//out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec2 offset;
uniform vec4 color;

void main()
{
    float scale = 2.0f;
    TexCoord = vertex.zw;
//    ParticleColor = color;
    gl_Position = projection * view * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
//    gl_Position = projection * view * mat4(1.0f) * vec4((vertex.xy + offset), 0.0, 1.0);
}
