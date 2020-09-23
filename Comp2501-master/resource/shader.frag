// Source code of fragment shader
//https://stackoverflow.com/questions/15693014/changing-level-of-opacity-in-opengl-es-2-0
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;
uniform float Opacity; // range 0.0 to 1.0

out vec4 FragColor;

uniform sampler2D onetex;
uniform vec3 colorMod;

void main()
{
	vec4 color = texture2D(onetex, uv_interp);
	FragColor = vec4(color.r,color.g,color.b,color.a);
	FragColor += vec4(colorMod, 0.0);					//the fragment color is modified by the 'color' uniform
	//FragColor.a*=Opacity;
    //if((color.r + color.g + color.b) / 3.0 <=0.10)
	if(color.a==0)
	{
		discard;
	}
//	 FragColor = color_interp;
}
