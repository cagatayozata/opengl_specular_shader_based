#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec3 in_Normal;
layout(location=3) in vec2 in_Texture;


out vec4 ex_Color;
out vec2 ex_Texture;
out vec3 ex_Normal;
out vec4 ex_Position ;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix ;
uniform mat4 ViewMatrix ;

void main(void)
{
  gl_Position = ProjectionMatrix * ModelViewMatrix * in_Position;
  ex_Position =  ModelViewMatrix * in_Position;
  ex_Color = in_Color ; 
  ex_Texture = in_Texture ;
  ex_Normal = NormalMatrix * in_Normal ;
}