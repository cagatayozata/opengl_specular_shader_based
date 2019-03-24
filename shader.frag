#version 400

in vec4 ex_Color;
in vec2 ex_Texture;
in vec3 ex_Normal ;
in vec4 ex_Position ;

uniform sampler2D Texture0 ;
uniform sampler2D Texture1 ;
out vec4 out_Color;

uniform float offset ;

void main(void)
{
  vec4 lightPos =  vec4(0,0,0,1) ; ;
  vec2 newTex = vec2(ex_Texture.x + offset, ex_Texture.y );
  vec4 outTexture = texture(Texture0, ex_Texture)  * ex_Color +  0.2 * texture(Texture1, newTex ); 

  out_Color = abs(dot( ex_Normal,  normalize( lightPos.xyz - ex_Position.xyz ))) * vec4(1) ; 
  
}