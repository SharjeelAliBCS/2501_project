/*---------------------------------------------------------------------------------*/
// Fragments of code for particle system
// DO NOT RUN -- integrate into your own codebase
/*---------------------------------------------------------------------------------*/

// Particle system vertex shader
// DOES NOT REPLACE OLD VERTEX SHADER -- you will want both
//
// This version produces a parabolic trajectory (used for water fountain)
//
const char *source_vpart = "#version 130\n\
\n\
// Vertex buffer\n\
in vec2 vertex;\n\
in vec2 dir;\n\
in float t;\n\
in vec2 uv;\n\
out vec2 uv_interp;\n\
\n\
// Uniform (global) buffer\n\
uniform mat4 x;\n\
uniform float time;\n\
\n\
// Attributes forwarded to the fragment shader\n\
out vec4 color_interp;\n\
\n\
\n\
void main()\n\
{\n\
	vec4 ppos;\n\
	float acttime;\n\
	float speed = 14.0;\n\
	float gravity = -9.8;\n\
	acttime = mod(time + t*10, 10.0);\n\
//	acttime = mod(time,10);\n\
    ppos = vec4(vertex.x+dir.x*acttime*speed , vertex.y+dir.y*acttime*speed
+ 0.5*gravity*acttime*acttime, 0.0, 1.0);\n\
    gl_Position = x*ppos;\n\
    \n\
    color_interp = vec4(uv,0.5, 1.0);\n\
    uv_interp = uv;\n\
}";



/*---------------------------------------------------------------------------------*/
// A setup function for the particle shader program
// Note, you have something like this already for the regular sprites --
// YOU NEED BOTH, this is not a replacement.
/*---------------------------------------------------------------------------------*/
GLuint SetupParticleShaders() // returns ID of newly created program
{

	// Set up shaders

	// Create a shader from vertex program source code
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &source_vpart, NULL);
	glCompileShader(vs);

	// Check if shader compiled successfully
	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
	   char buffer[512];
	   	glGetShaderInfoLog(vs, 512, NULL, buffer);
				       throw(std::ios_base::failure(std::string("Error compiling vertex shader:
") + std::string(buffer)));
   }

   // Create a shader from the fragment program source code
   GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fs, 1, &source_fp, NULL);
   glCompileShader(fs);

   // Check if shader compiled successfully
   glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
   if (status != GL_TRUE) {
      char buffer[512];
      	   glGetShaderInfoLog(fs, 512, NULL, buffer);
	   			  throw(std::ios_base::failure(std::string("Error compiling fragment
shader: ") + std::string(buffer)));
	}

	// Create a shader program linking both vertex and fragment shaders
	// together
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	// Check if shaders were linked successfully
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
	   char buffer[512];
	   	glGetShaderInfoLog(program, 512, NULL, buffer);
					    throw(std::ios_base::failure(std::string("Error linking shaders: ") +
std::string(buffer)));
	}

	// Delete memory used by shaders, since they were already compiled
	// and linked
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

/*---------------------------------------------------------------------------------*/
// Attribute binding for the current program -- note that the vertex data layout
// has changed; I changed it for all vertices (including regular sprites)
// because I was not using the color attribute, but if you are, you will need
// to further change the layout or create a second layout and switch between them
/*---------------------------------------------------------------------------------*/
void AttributeBinding(GLuint program)
{

	// Set attributes for shaders
	// Should be consistent with how we created the buffers for the particle
elements
	GLint vertex_att = glGetAttribLocation(program, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 *
sizeof(GLfloat), 0);
		 glEnableVertexAttribArray(vertex_att);

		 GLint dir_att = glGetAttribLocation(program, "dir");
		 glVertexAttribPointer(dir_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
(void *)(2 * sizeof(GLfloat)));
      glEnableVertexAttribArray(dir_att);

      GLint time_att = glGetAttribLocation(program, "t");
      glVertexAttribPointer(time_att, 1, GL_FLOAT, GL_FALSE, 7 *
sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
		 glEnableVertexAttribArray(time_att);

		 GLint tex_att = glGetAttribLocation(program, "uv");
		 glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
(void *)(5 * sizeof(GLfloat)));
      glEnableVertexAttribArray(tex_att);

}

/*---------------------------------------------------------------------------------*/
// Creates the particle data -- note data layout and initialization
// Each particle has four vertices (same as Square) and attributes -- direction,
// aka velocity, and "time" (a per-particle random phase constant) -- set on a
// per-particle basis
//
// Current initialization puts the velocities into a narrow angular band,
// but you can easily modify it to get a circle, or change it completely
// if you want something totally different
//
/*---------------------------------------------------------------------------------*/
int CreateParticleArray(void) {

    // Each particle is a square with four vertices and two triangles

    // Number of attributes for vertices and faces
       const int vertex_attr = 7;  // 7 attributes per vertex: 2D (or 3D)
position (2), direction (2), 2D texture coordinates (2), time (1)
	 //   const int face_att = 3; // Vertex indices (3)

	 GLfloat vertex[] = {
	 	 //  square (two triangles)
		     //  Position      Color             Texcoords
		     	 -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
			 	0.5f, 0.5f,   	     0.0f, 1.0f, 0.0f,	      1.0f, 0.0f, // Top-right
				      0.5f, -0.5f,    0.0f, 0.0f, 1.0f,	      	    1.0f, 1.0f, // Bottom-right
				      	    -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,	    	  0.0f, 1.0f  // Bottom-left
					    };

					    GLfloat particleatt[1000*vertex_attr];
					    float theta, r, tmod;

					    for (int i = 0; i < 1000; i++)
					    {
						if (i % 4 == 0)
						   {
								theta = (2*(rand() % 10000) / 10000.0f -1.0f)*0.13f;
								      	r = 0.7f + 0.3*(rand() % 10000) / 10000.0f;
									    tmod = (rand() % 10000) / 10000.0f;
									    	 }
                  // set position
		  particleatt[i*vertex_attr + 0] = vertex[(i % 4)*7 + 0];
		  particleatt[i*vertex_attr + 1] = vertex[(i % 4)*7 + 1];
		  
                  // set velocity
 		  particleatt[i*vertex_attr + 2] = sin(theta)*r; 	
                  particleatt[i*vertex_attr + 3] = cos(theta)*r;

                  // set phase 
  		  particleatt[i*vertex_attr + 4] = tmod;
            
                  // set texture coordinates
                  particleatt[i*vertex_attr + 5] = vertex[(i % 4)*7 + 5];
		  particleatt[i*vertex_attr + 6] = vertex[(i % 4)*7 + 6];

	}


	GLuint face[] = {
			0, 1, 2, // t1
			2, 3, 0  //t2
			};

			GLuint manyface[1000 * 6];

			for (int i = 0; i < 1000; i++)
			{
			for (int j = 0; j < 6; j++)
				manyface[i * 6 + j] = face[j]+i*4;
			}

			GLuint vbo, ebo;

			// Create buffer for vertices
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(particleatt), particleatt,GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manyface), manyface,GL_STATIC_DRAW);

	// Return number of elements in array buffer
	return sizeof(manyface);

}
/*---------------------------------------------------------------------------------*/
// draw all particles -- note that the transformation is just hacked in here,
// but you can set it in your own code (e.g., to attach a particle system to
// one of the cars)
/*---------------------------------------------------------------------------------*/
void drawParticles(GLuint particleprogram, int particlesize)
{

	// Select proper shader program to use
	glUseProgram(particleprogram);

	//set displacement
	int matrixLocation = glGetUniformLocation(particleprogram, "x");
	int timeLocation = glGetUniformLocation(particleprogram, "time");

	glm::mat4 rot = glm::mat4();
	glm::mat4 world = glm::mat4();

	float k = glfwGetTime();
	//rot = glm::rotate(rot, -k * 360 / 6.283f, glm::vec3(0, 0, 1));
	rot = glm::translate(rot, glm::vec3(0.5, 0, 0));
	rot = glm::scale(rot, glm::vec3(0.1, 0.1, 0.1));
	// get ready to draw, load matrix
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &rot[0][0]);
	glUniform1f(timeLocation, k);
	glBindTexture(GL_TEXTURE_2D, tex[1]);

	// Draw 
	glDrawElements(GL_TRIANGLES, 6*particlesize, GL_UNSIGNED_INT, 0);

}

/*---------------------------------------------------------------------------------*/
// Something else you might want to do:
// - set additive mode for blending (for a fire effect, say)
// - to do this, call glBlendFunc(GL_ONE,GL_ONE);
// - and also disable depth writes: glDepthMask(GL_FALSE);
// - disable depth writes RIGHT BEFORE drawing the particles,
// - and reenable them afterwards with glDepthMask(GL_TRUE);
/*---------------------------------------------------------------------------------*/
	 glBlendFunc(GL_ONE, GL_ONE);

/*---------------------------------------------------------------------------------*/
// Couple other basic things:
//
// Create two shader programs, one for regular objects and one for the particle system
// (they can share a fragment shader, although they don't have to)
//
// Make sure you are using the proper shader for your objects;
// compile the shader programs once only, and save the program ID
// so you can later call glUseProgram(correctProgramID);
// after setting the program, subsequent draw calls will use that
// shader set
//
// Create the square and particle set using the supplied functions
// before trying to draw anything
//
// Your existing vertex shader program may be out of synch with the new vertex data layout --
// rewrite the header like so:
// ----
// Source code of vertex shader
const char *source_vp = "#version 130\n\
\n\
// Vertex buffer\n\
in vec2 vertex;\n\
in vec2 dir;\n\
in float t;\n\
in vec2 uv;\n\
out vec2 uv_interp;\n\
\n\
/*---------------------------------------------------------------------------------*/