// Source code of fragment shader
#version 130

// Vertex buffer
in vec2 vertex;
in vec2 dir;
in float t;
in vec2 uv;
out vec2 uv_interp;
out float tout;
out float timeout;
out float outrange;

// Uniform (global) buffer
uniform mat4 x;
uniform float time;
uniform mat4 viewMatrix;
uniform float range;

// Attributes forwarded to the fragment shader
out vec4 color_interp;



void main()
{

	vec4 ppos;
	float acttime;
	float maxTime = 2.0*range;
	float speed = 9.0;
	float gravity = -9.8;
	acttime = mod(time + t*maxTime, maxTime);
//	acttime = mod(time,4);
    ppos = vec4(vertex.x+dir.x*acttime*speed , vertex.y+dir.y*acttime*speed - 0.5*gravity*acttime*acttime, 0.0, 1.0);
    gl_Position = viewMatrix*x*ppos;
	// gl_Position = viewMatrix * transformationMatrix * vertexPos;
	
//   color_interp = vec4(0.5+0.5*cos(4*acttime),0.5*sin(4*acttime)+0.5,0.5, 1.0);
    color_interp = vec4(uv,0.5,1.0);
	

	if(acttime>=range)
		uv_interp =vec2(uv.x/(acttime*maxTime-range),uv.y/(acttime*maxTime-range));
	else
		uv_interp = uv;
	tout = t;
	timeout = time;
	outrange = range;
}
