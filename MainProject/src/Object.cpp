#include <Object.hpp>

Object::Object(GLfloat* vertices, GLuint* indices, glm::vec3 pos, const char* pathToImg, glm::vec4 lightColor, glm::vec3 lightPos)
{
    VAO1.Bind(); // Binds it
	VBO1 = VBO(vertices, sizeof(vertices)); // Generates Vertex Buffer Object and links it to vertices
	EBO1 = EBO(indices, sizeof(indices)); // Generates Element Buffer Object and links it to indices
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11*sizeof(float), (void*)0); // Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11*sizeof(float), (void*)(3*sizeof(float))); // Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11*sizeof(float), (void*)(6*sizeof(float))); // Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11*sizeof(float), (void*)(8*sizeof(float)));
	VAO1.Unbind(); // Unbind to prevent accidentally modifying
	VBO1.Unbind(); // Unbind to prevent accidentally modifying
	EBO1.Unbind(); // Unbind to prevent accidentally modifying

	Model = glm::translate(Model, pos);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Texture picture(pathToImg, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	picture.texUnit(shaderProgram, "tex0", 0);
    indicesSize = sizeof(indices);
}

void Object::draw(Camera camera)
{
    shaderProgram.Activate(); // Tell OpenGL which Shader Program we want to use
	//picture.Bind();
	VAO1.Bind(); // Bind the VAO so OpenGL knows to use it
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z); // Exports the camera Position to the Fragment Shader for specular lighting
	glDrawElements(GL_TRIANGLES, indicesSize/sizeof(int), GL_UNSIGNED_INT, 0); // Draw primitives, number of indices, datatype of indices, index of indices
    camera.Matrix(shaderProgram, "camMatrix"); // Export the camMatrix to the Vertex Shader of the light cube
}

void Object::setPosition(glm::vec3 pos)
{

}

void Object::setRotation(glm::vec3 rot)
{

}

void Object::move(glm::vec3 rot)
{

}

void Object::rotate(glm::vec3 rot)
{

}

glm::vec3 Object::getPosition()
{
    return Position;
}

glm::vec3 Object::getRotation()
{
    return Orientation;
}
