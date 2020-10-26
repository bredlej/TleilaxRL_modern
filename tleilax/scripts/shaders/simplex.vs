varying vec2 v_texCoord2D;
varying vec3 v_texCoord3D;
varying vec4 v_color;
void main()
{	
  gl_Position = gl_ModelViewProjectionMatrix * 
                   gl_Vertex.xyz;
  v_texCoord2D = gl_MultiTexCoord0.xy;
  v_texCoord3D = gl_Vertex.xyz * 0.05;
  v_color = gl_Color;  
}