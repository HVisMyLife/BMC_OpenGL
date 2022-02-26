#ifndef OBJECT_CLASS_H // include guard
#define OBJECT_CLASS_H

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shaderClass.hpp>
#include <VAO.hpp>
#include <VBO.hpp>
#include <EBO.hpp>
#include <Camera.hpp>
#include <Texture.hpp>

class Object{
private:
    glm::mat4 Model = glm::mat4(1.0f);
    glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    unsigned long indicesSize;
    Shader shaderProgram = Shader("../../resources/shaders/default.vert", "../../resources/shaders/default.frag"); // Generates Shader object using shaders defualt.vert and default.frag
    VAO VAO1; // Generates Vertex Array Object
    VBO VBO1; // Generates Vertex Buffer Object and links it to vertices
	EBO EBO1; // Generates Element Buffer Object and links it to indices
public:
    Object(GLfloat*, GLuint*, glm::vec3, const char*, glm::vec4, glm::vec3);
    void setPosition(glm::vec3);
    void setRotation(glm::vec3);
    void move(glm::vec3);
    void rotate(glm::vec3);
    glm::vec3 getPosition(void);
    glm::vec3 getRotation(void);
    void draw(Camera);
};

#endif