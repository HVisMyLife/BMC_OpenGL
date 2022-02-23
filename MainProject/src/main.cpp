#include <iostream>
#include <stdio.h>
#include <cmath>
#include <string>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <player.hpp>
#include <shaderClass.hpp>
#include <VAO.hpp>
#include <VBO.hpp>
#include <EBO.hpp>
#include <Texture.hpp>
#include <Camera.hpp>
#include <vars.hpp>


#define WNDW_WIDTH 1920
#define WNDW_HEIGHT 1080
#define GLFW_REPEAT   2


namespace fs = std::filesystem;
using std::string;


GLFWwindow *window;
bool quit = false;
static bool s_showStats = false;
double lastTime = glfwGetTime();
int nbFrames = 0;


static uint64_t GetTickCount();
void CalculateFrameRate();
void trash(GLFWwindow*);
void key_callback(GLFWwindow*, int, int, int, int);
static void cursor_position_callback(GLFWwindow*, double, double);
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

int init(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);
	
	window = glfwCreateWindow(WNDW_WIDTH, WNDW_HEIGHT, "YoutubeOpenGL", NULL, NULL);
	if (window == NULL) { std::cout << "Failed to create GLFW window" << std::endl; glfwTerminate(); return -1; }
	glfwMakeContextCurrent(window); // Introduce the window into the current context

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cout << "Failed to initialise GLAD" << std::endl; }
	glViewport(0, 0, WNDW_WIDTH, WNDW_HEIGHT); // Specify the viewport of OpenGL in the Window
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Specify the color of the background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the back buffer and assign the new color to it

	glfwSetKeyCallback(window, key_callback); // Keybord handling
  	glfwSetCursorPosCallback(window, cursor_position_callback); // Mouse handling
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Window size handling

	return 0;
}

int main(void)
{
	init();
	Player player = Player(10,10,25,30);
	
	Shader shaderProgram("../../resources/shaders/default.vert", "../../resources/shaders/default.frag"); // Generates Shader object using shaders defualt.vert and default.frag
	VAO VAO1; // Generates Vertex Array Object
	VAO1.Bind(); // Binds it
	VBO VBO1(vertices, sizeof(vertices)); // Generates Vertex Buffer Object and links it to vertices
	EBO EBO1(indices, sizeof(indices)); // Generates Element Buffer Object and links it to indices
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11*sizeof(float), (void*)0); // Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11*sizeof(float), (void*)(3*sizeof(float))); // Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11*sizeof(float), (void*)(6*sizeof(float))); // Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11*sizeof(float), (void*)(8*sizeof(float)));
	VAO1.Unbind(); // Unbind to prevent accidentally modifying
	VBO1.Unbind(); // Unbind to prevent accidentally modifying
	EBO1.Unbind(); // Unbind to prevent accidentally modifying

	Shader lightShader("../../resources/shaders/light.vert", "../../resources/shaders/light.frag"); // Shader for light cube
	VAO lightVAO; // Generates Vertex Array Object
	lightVAO.Bind(); // Binds it
	VBO lightVBO(lightVertices, sizeof(lightVertices)); // Generates Vertex Buffer Object and links it to vertices
	EBO lightEBO(lightIndices, sizeof(lightIndices)); // Generates Element Buffer Object and links it to indices
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0); // Links VBO attributes such as coordinates and colors to VAO
	lightVAO.Unbind(); // Unbind to prevent accidentally modifying
	lightVBO.Unbind(); // Unbind to prevent accidentally modifying
	lightEBO.Unbind(); // Unbind to prevent accidentally modifying


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	Texture picture("../../resources/images/img.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	picture.texUnit(shaderProgram, "tex0", 0);

	Camera camera(WNDW_WIDTH, WNDW_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
	

	while (!quit && !glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		trash(window);
		CalculateFrameRate();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the back buffer and assign the new color to it

		shaderProgram.Activate(); // Tell OpenGL which Shader Program we want to use
		picture.Bind();
		VAO1.Bind(); // Bind the VAO so OpenGL knows to use it
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		camera.Matrix(shaderProgram, "camMatrix");
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z); // Exports the camera Position to the Fragment Shader for specular lighting
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0); // Draw primitives, number of indices, datatype of indices, index of indices

		
		lightShader.Activate(); // Tells OpenGL which Shader Program we want to use
		camera.Matrix(lightShader, "camMatrix"); // Export the camMatrix to the Vertex Shader of the light cube
		lightVAO.Bind(); // Bind the VAO so OpenGL knows to use it
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0); // Draw primitives, number of indices, datatype of indices, index of indices

		glfwSwapBuffers(window);
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	picture.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void trash(GLFWwindow* window){
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) printf("Spacing...lol;)XD\n");
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {printf("MESSAGE: <ESC> is pressed...\n"); quit=true;}
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {printf("MESSAGE: <SPACE> is pressed...\n");}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  //printf("X %f Y %f\n", xpos, ypos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void CalculateFrameRate()
{
	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;
	if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f fps\n", double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}

static uint64_t GetTickCount()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_nsec / 1000000) + ((uint64_t)ts.tv_sec * 1000ull);
}