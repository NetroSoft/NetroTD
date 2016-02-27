#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

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

    /*
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

*/



    std::string inputfile = "models/rungholt/rungholt.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

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



    GLuint program;
    GLuint vbo_triangle;	
    GLint attribute_coord2d;
    GLuint vaoIds[shapes.size()];
    GLuint vboIds[shapes.size()];
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

    for(int i = 0; i < shapes.size(); ++i)
    {
        glGenVertexArrays(1,&vaoIds[i]);
        glBindVertexArray(vaoIds[i]);
        
        glGenBuffers(1,&vboIds[i]);
        glBindBuffer(GL_ARRAY_BUFFER,vboIds[i]);
        glBufferData(GL_ARRAY_BUFFER,shapes[i].mesh.num_vertices.size() * sizeof(shapes[i].mesh.num_vertices[0]),&shapes[i].mesh.num_vertices[0], GL_STATIC_DRAW);
    }
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

        //glColor3f(0,1,0);
        for(int i = 0; i < shapes.size();++i)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vboIds[i]);
            //glEnableVertexAttribArray(attribute_coord2d);

            // Draw
            glDrawArrays(GL_TRIANGLES, 0, shapes[i].mesh.num_vertices.size());
        }
        glBindVertexArray(0);
        //glDisableVertexAttribArray(attribute_coord2d);
        glUseProgram(0);

        // Refresh window
        window.display();
    }


}
