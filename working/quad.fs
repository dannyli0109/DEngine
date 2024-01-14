#version 330

out vec4 f_Color;

in vec4 v_Color;
in vec2 v_Uvs;
in vec3 v_Position;
flat in float v_TextureIndex;

uniform sampler2D u_Textures[16];


void main()
{

	vec4 texColor = v_Color;
	switch(int(v_TextureIndex))
	{
		case 0:  texColor *= texture(u_Textures[0], v_Uvs); break;
		case 1:  texColor *= texture(u_Textures[1],  v_Uvs); break;
		case 2:  texColor *= texture(u_Textures[2],  v_Uvs); break;
		case 3:  texColor *= texture(u_Textures[3],  v_Uvs); break;
		case 4:  texColor *= texture(u_Textures[4],  v_Uvs); break;
		case 5:  texColor *= texture(u_Textures[5],  v_Uvs); break;
		case 6:  texColor *= texture(u_Textures[6],  v_Uvs); break;
		case 7:  texColor *= texture(u_Textures[7],  v_Uvs); break;
		case 8:  texColor *= texture(u_Textures[8],  v_Uvs); break;
		case 9:  texColor *= texture(u_Textures[9],  v_Uvs); break;
		case 10: texColor *= texture(u_Textures[10], v_Uvs); break;
		case 11: texColor *= texture(u_Textures[11], v_Uvs); break;
		case 12: texColor *= texture(u_Textures[12], v_Uvs); break;
		case 13: texColor *= texture(u_Textures[13], v_Uvs); break;
		case 14: texColor *= texture(u_Textures[14], v_Uvs); break;
		case 15: texColor *= texture(u_Textures[15], v_Uvs); break;
	}
	f_Color = texColor;
}