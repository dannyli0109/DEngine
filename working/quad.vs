#version 330

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_Uvs;
layout (location = 3) in float a_TexureIndex;

out vec4 v_Color;
out vec2 v_Uvs;
flat out float v_TextureIndex;
out vec3 v_Position;

uniform mat4 u_ProjectionMatrix;	//From object space into world space.
void main()
{
	v_Color = a_Color;
	v_Uvs = a_Uvs;
	
	v_TextureIndex = a_TexureIndex;
	v_Position = a_Position;
	
	gl_Position = u_ProjectionMatrix * vec4(a_Position, 1);
}