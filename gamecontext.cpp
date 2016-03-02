#include "gamecontext.h"
#include "shader.h"
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

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(program);

        
        glClearColor(0,0,0,1);

        glColor3f(0,1,0);
        for(int i = 0; i < shapes.size();++i)
        {
             glBindBuffer(GL_ARRAY_BUFFER, vboIds[i]);
             glDrawArrays(GL_TRIANGLES, 0, shapes[i].mesh.num_vertices.size());
        }

        glBindVertexArray(0);
        glUseProgram(0);

        
        // Refresh window
        m_window.display();
        }
    }

	return 1;
}

void GameContext::initializeComponents()
{

	// Load glew
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
		throw std::runtime_error("Glewinit");


    //Load les shaders
    program = Shader::Load("TransformVertexShader.vertexshader",
            "TextureFragmentShader.fragmentshader");
	
    //Load le obj de la map
    loadMap();
    
    // Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);    
	
    // Some useful info
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
}

void GameContext::loadMap()
{
    std::string inputfile = "models/rungholt/rungholt.obj";

 
     std::string err;
     bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(),"models/rungholt/materials/");
 
     if (!err.empty()) { // `err` may contain warning message.
         std::cerr << err << std::endl;
     }
 
     if (!ret) {
         exit(1);
     }
 
     std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
 
     for (size_t i = 0; i < shapes.size(); i++) 
     {
         printf("shape[%ld].name = %s\n", 
                 i, shapes[i].name.c_str());
         printf("Size of shape[%ld].indices: %ld\n", 
                 i, shapes[i].mesh.indices.size());
         printf("Size of shape[%ld].material_ids: %ld\n", 
                 i, shapes[i].mesh.material_ids.size());
 
         assert((shapes[i].mesh.indices.size() % 3) == 0);
 
         for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) 
        {
             printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", 
                     f, shapes[i].mesh.indices[3*f+0], 
                     shapes[i].mesh.indices[3*f+1], 
                     shapes[i].mesh.indices[3*f+2], 
                     shapes[i].mesh.material_ids[f]);
         }
 
         printf("shape[%ld].vertices: %ld\n", 
                 i, shapes[i].mesh.positions.size());
 
         assert((shapes[i].mesh.positions.size() % 3) == 0);
 
         for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) 
         {
             printf("  v[%ld] = (%f, %f, %f)\n", v,
                     shapes[i].mesh.positions[3*v+0],
                     shapes[i].mesh.positions[3*v+1],
                     shapes[i].mesh.positions[3*v+2]);
         }
     }
 
     for (size_t i = 0; i < materials.size(); i++) {
         printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
         printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
         printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
         printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
         printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
         printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
         printf("  material.Ns = %f\n", materials[i].shininess);
         printf("  material.Ni = %f\n", materials[i].ior);
         printf("  material.dissolve = %f\n", materials[i].dissolve);
         printf("  material.illum = %d\n", materials[i].illum);
         printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
         printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
         printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
         printf("  material.map_Ns = %s\n", materials[i].specular_highlight_texname.c_str());
         std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
         std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
         for (; it != itEnd; it++) {
             printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
         }
         printf("\n");
     }	
    

    vaoIds.resize(shapes.size());
    vboIds.resize(shapes.size());

     for(int i = 0; i < shapes.size(); ++i)
     {
         glGenVertexArrays(1,&vaoIds[i]);
         glBindVertexArray(vaoIds[i]);
         
         glGenBuffers(1,&vboIds[i]);
         glBindBuffer(GL_ARRAY_BUFFER,vboIds[i]);
         glBufferData(GL_ARRAY_BUFFER,shapes[i].mesh.num_vertices.size() * sizeof(shapes[i].mesh.num_vertices[0]),&shapes[i].mesh.num_vertices[0], GL_STATIC_DRAW);
     }
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
	return m_elapsed_time;}

sf::Vector2i GameContext::getMousePosition() const
{
    return  sf::Mouse::getPosition(m_window);
}


