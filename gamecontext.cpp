#include "gamecontext.h"
#include <iostream>

GameContext::GameContext()
:	m_config("NetroTD.cfg")
{
	m_config.load();
}

bool GameContext::start(std::string& title)
{

	m_window.create(
			sf::VideoMode(
				m_config.getInt("window_width"),
				m_config.getInt("window_height")),
			title, sf::Style::Default);

	initializeComponents();

	sf::Clock clock;

	while(m_window.isOpen())
	{
		clock.restart();
		sf::Event event;

		while(m_window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed:
				{
					m_window.close();
					glDeleteProgram(program);
					return EXIT_SUCCESS;
				}
				case sf::Event::MouseMoved:
				case sf::Event::KeyPressed:
				{
					break;
				}
				case sf::Event::Resized:
				{
					glViewport(0, 0, event.size.width, event.size.height);
					break;
				}
			}

		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(program);


		glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
		glEnableVertexAttribArray(attribute_coord2d);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(attribute_coord2d);
		glUseProgram(0);
		// Refresh window
		m_window.display();
	}

	return 1;
}

void GameContext::initializeComponents()
{

	// Load glew
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
		throw std::runtime_error("Glewinit");
	this->loadShader();
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


	//test stuff to remove
	//Buffer
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	static const GLfloat triangle_vertices[] =
	{
		0.0,	0.8,
		-0.8,	-0.8,
		0.8,	-0.8,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);


	// Some useful info
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;


}

void GameContext::loadShader()
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

void GameContext::centerMouse()
{
	sf::Vector2i v(
			this->getWindowWidth() / 2,
			this->getWindowHeight() / 2);

	sf::Mouse::setPosition(v, m_window);
}

int GameContext::getWindowWidth() const
{
	return m_window.getSize().x;
}

int GameContext::getWindowHeight() const
{
	return m_window.getSize().y;
}

float GameContext::getElapsedTime() const
{
	return m_elapsed_time;
}

sf::Vector2i GameContext::getMousePosition() const
{
	return  sf::Mouse::getPosition(m_window);
}
