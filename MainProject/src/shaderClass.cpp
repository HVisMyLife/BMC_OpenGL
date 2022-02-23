#include <shaderClass.hpp>

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Check for errors
void errorCheck(uint shader, const char* type)
{
	int  success;
	char infoLog[512];
	if (type != "PROGRAM"){
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile); // Read vertexFile and store the string
	std::string fragmentCode = get_file_contents(fragmentFile); // Read fragmentFile and store the string
	
	const char* vertexSource = vertexCode.c_str(); // Convert the shader source string into character array
	const char* fragmentSource = fragmentCode.c_str(); // Convert the shader source string into character array

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create Vertex Shader Object and get its reference
	glShaderSource(vertexShader, 1, &vertexSource, NULL); // Attach Vertex Shader source to the Vertex Shader Object
	glCompileShader(vertexShader); // Compile the Vertex Shader into machine code
	errorCheck(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create Fragment Shader Object and get its reference
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL); // Attach Fragment Shader source to the Fragment Shader Object
	glCompileShader(fragmentShader); // Compile the Vertex Shader into machine code
	errorCheck(fragmentShader, "FRAGMENT");

	ID = glCreateProgram(); // Create Shader Program Object and get its reference
	glAttachShader(ID, vertexShader); // Attach the Vertex Shader to the Shader Program
	glAttachShader(ID, fragmentShader); // Attach the Fragment Shader to the Shader Program
	glLinkProgram(ID); // Wrap-up/Link all the shaders together into the Shader Program
	errorCheck(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}