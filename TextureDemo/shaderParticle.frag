// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;
in float tout;
in float timeout;
in float inrange;
out vec4 FragColor;

uniform sampler2D onetex;
uniform vec3 colorMod;
uniform float Opacity; // range 0.0 to 1.0

float opacity = 1.0f;
void main()
{

	float acttime;
	acttime = mod(timeout + tout*2.0*inrange, 2.0*inrange);
	
	vec4 color = texture2D(onetex, uv_interp);
	FragColor = vec4(color.r,color.g,color.b,color.a);
	FragColor += vec4(colorMod, 0.0);					//the fragment color is modified by the 'color' uniform
	if(acttime>=1)
		FragColor.a=(2-acttime);
    //if((color.r + color.g + color.b) / 3.0 <=0.10)
	if(color.a==0)
	{
		discard;
	}
//	 FragColor = color_interp;
}
