#version 330 core
out vec4 FragColor;
  
//in vec3 ourColor;
in vec2 TexCoord;
//in vec4 ParticleColor;

uniform sampler2D ourTexture;

void main()
{
//    FragColor = (texture(ourTexture, TexCoord) * ParticleColor);
        FragColor = (texture(ourTexture, TexCoord) );
//    FragColor = vec4(TexCoord, 0.0f, 1.0f);
}
