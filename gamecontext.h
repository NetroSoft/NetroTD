#ifndef GAMECONTEXT_H__
#define GAMECONTEXT_H__

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

#include <vector>
#include "config.h"

#include "tiny_obj_loader.h"

class GameContext
{

//Singleton
public:
	static GameContext& Instance()
	{
		static GameContext instance;
		return instance;
	}

	//Public delete function to provide better error message
	//due to the compiler behavior to check accessibility
	//before deleted status
	GameContext(GameContext const&) = delete;
	void operator=(GameContext const&) = delete;

private:
	GameContext();

//Functions
public:
	int getWindowWidth() const;
	int getWindowHeight() const;

	sf::Vector2i getMousePosition() const;

	float getElapsedTime() const;


	bool start(std::string & title);

	void centerMouse();
   
    void initializeComponents();
    void loadMap();
//Member
private:
	sf::Window 	m_window;

	float 		m_elapsed_time;
	Config 		m_config;


    //TO REMOVE (TEST)
    GLuint program;
    GLuint vbo_triangle;	
    GLint attribute_coord2d;

     std::vector<tinyobj::shape_t> shapes;
     std::vector<tinyobj::material_t> materials;

     std::vector<GLuint> vaoIds;
     std::vector<GLuint> vboIds;
};

#endif // !GAMECONTEXT_H__
