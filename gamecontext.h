#ifndef GAMECONTEXT_H__
#define GAMECONTEXT_H__

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
class GameContext{
//Singleton start
    public:
        static GameContext & Instance()
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
        GameContext(){};
//Singleton end


//Functions
    public:
        int getWindowWidth() const;
        int getWindowHeight() const;

        sf::Vector2i getMousePosition() const;

        float getElapsedTime() const;
        
    
        bool start(std::string & title);
               
        void centerMouse();
    
        void initializeComponents();
        void loadShader();

    private:
        sf::Window m_window;

        float m_elapsed_time;


        //TO REMOVE (TEST)
        GLuint program;
        GLuint vbo_triangle;	
        GLint attribute_coord2d;
};

#endif // !GAMECONTEXT_H__
