#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <iostream>


void loadShader(GLuint &program)
{
	GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

	// Vertex
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const char *vs_source =
		"#version 120\n"
		"attribute vec2 coord2d;                  "
		"void main(void) {                        "
		"  gl_Position = vec4(coord2d, 0.0, 1.0); "
		"}";
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok) 
		throw std::runtime_error("Error in vertex shader");

	// Fragment
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fs_source =
		"#version 120\n" 
		"void main(void) {        "
		"  gl_FragColor[0] = 0.3; "
		"  gl_FragColor[1] = 1.0; "
		"  gl_FragColor[2] = 0.3; "
		"}";
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok) 
		throw std::runtime_error("Error in fragment shader");

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) 
		throw std::runtime_error("Error in glLinkProgram");

	const char* attribute_name = "coord2d";
}

int main()
{
	GLuint program;
	GLuint vbo_triangle;	
	GLint attribute_coord2d;

	sf::Window window(sf::VideoMode(800,600),"NetroTD",sf::Style::Default);

	// Load glew
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) 
		throw std::runtime_error("Glewinit");

	// Load Shader
	loadShader(program);

	attribute_coord2d = glGetAttribLocation(program, "coord2d");
	if (attribute_coord2d == -1) 
		throw std::runtime_error("Could not bind attribute");

	//Buffer
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	static const GLfloat triangle_vertices[] = 
	{
		0.0,	0.8,
		-0.8,	-0.8,
		0.8, 	-0.8,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Some useful info
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;


	while (true)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
			{
				glDeleteProgram(program);
				return EXIT_SUCCESS;
			}

            else if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

		// Clear
		glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
		glEnableVertexAttribArray(attribute_coord2d);

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(attribute_coord2d);
		glUseProgram(0);

		// Refresh window
        window.display();
    }
}
