#version 330 core

out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 ourColor;

void main()
{
    // No color yet...
    // FragColor = texture(ourTexture, TexCoor) * vec4(ourColor, 1.0);
    FragColor = texture(ourTexture, TexCoord);
}