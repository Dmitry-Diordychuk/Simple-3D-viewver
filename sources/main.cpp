#define GL_SILENCE_DEPRECATION
#include <glad/glad.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "texture_loader.hpp"
#include "rt_vector.hpp"
#include "rt_matrix.hpp"

rt::RTMatrix<float> translate(
	rt::RTMatrix<float> identityMatrix,
	rt::RTVector<float> transVector
) {
	identityMatrix[0][3] = transVector[0];
	identityMatrix[1][3] = transVector[1];
	identityMatrix[2][3] = transVector[2];
	return identityMatrix;
}

rt::RTMatrix<float> rotate(
	rt::RTMatrix<float> iMat,
	float rad,
	rt::RTVector<float> rotAxis
) {
	iMat[0][0] = cos(rad) + rotAxis['x'] * rotAxis['x'] * (1 * cos(rad));
	iMat[1][0] = rotAxis['y'] * rotAxis['x'] * (1 - cos(rad)) + rotAxis['z'] * sin(rad);
	iMat[2][0] = rotAxis['z'] * rotAxis['x'] * (1 - cos(rad)) - rotAxis['y'] * sin(rad);
	iMat[3][0] = 0;

	iMat[0][1] = rotAxis['x'] * rotAxis['y'] * (1 - cos(rad)) - rotAxis['z'] * sin(rad);
	iMat[1][1] = cos(rad) + rotAxis['y'] * rotAxis['y'] * (1 - cos(rad));
	iMat[2][1] = rotAxis['z'] * rotAxis['y'] * (1 - cos(rad)) + rotAxis['x'] * sin(rad);
	iMat[3][1] = 0;

	iMat[0][2] = rotAxis['x'] * rotAxis['z'] * (1 - cos(rad)) + rotAxis['y'] * sin(rad);
	iMat[1][2] = rotAxis['y'] * rotAxis['z'] * (1 - cos(rad)) - rotAxis['x'] * sin(rad);
	iMat[2][2] = cos(rad) + rotAxis['z'] * rotAxis['z'] * (1 - cos(rad));
	iMat[3][2] = 0;

	iMat[0][3] = 0;
	iMat[1][3] = 0;
	iMat[2][3] = 0;
	iMat[3][3] = 1;

	return iMat;
}

rt::RTMatrix<float> scale(
	rt::RTMatrix<float> identityMatrix,
	rt::RTVector<float> scaleVector
) {
	identityMatrix[0][0] = scaleVector['x'];
	identityMatrix[1][1] = scaleVector['y'];
	identityMatrix[2][2] = scaleVector['z'];
	return identityMatrix;
}

void error_callback(int error, const char* description)
{
    std::cerr << "Error " << error << ": " << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
/////////////////////////Window/////////////////////////////////////////////////
	if (!glfwInit()) {
		std::cerr << "GLFW initialization failed" << std::endl;
		return (-1);
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Scop", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return (-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

/////////////////////Bind Opengl functions//////////////////////////////////////

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return (-1);
	}

	glViewport(0, 0, 800 * 2, 600 * 2); // * 2 - macbook retina display

////////////////////////Create shaders//////////////////////////////////////////

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 transform;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = transform * vec4(aPos, 1.0);\n"
		"	ourColor = aColor;\n"
		"	TexCoord = aTexCoord;\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glfwTerminate();
		return (-1);
	}

	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"   FragColor = texture(ourTexture, TexCoord);\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		glfwTerminate();
		return (-1);
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		glfwTerminate();
		return (-1);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


///////////////////////////Texture//////////////////////////////////////////////

	Scop::TextureLoader textLoader("water.bmp");
	unsigned char* image = textLoader.getPixelArray();
	size_t width = textLoader.getWidth();
	size_t height = textLoader.getHeight();

////// 					Check texture 					/////
	// const size_t width = 64;
	// const size_t height = 64;
	// GLubyte image[width][height][3];
	// int i, j, c;
	// for (i = 0; i < width; i++) {
	// 	for (j = 0; j < height; j++) {
	// 		c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
	// 		image[i][j][0] = (GLubyte) c;
	// 		image[i][j][1] = (GLubyte) c;
	// 		image[i][j][2] = (GLubyte) c;
	// 	}
	// }

	unsigned int texture;
	glGenTextures(1, & texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "Texture loading failed!" << std::endl;
		return -1;
	}

////////////////////////////////////////////////////////////////////////////////

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		0.5f, 1.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int indices[] = {
		0, 1, 2
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture"), 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

/////////////// Transformation matrix //////////////////////////////////////////
	rt::RTMatrix<float> trans(4, 4);

	trans.toIdentity();
	trans = translate(trans, rt::RTVector<float>(1.0f, 0.0f, 0.0f));
	// vec = trans * vec;
	// std::cout << vec['x'] << vec['y'] << vec['z'] << std::endl;

	//trans.toIdentity();
	//trans = rotate(trans, 90.0f, rt::RTVector<float>(0.0f, 0.0f, 1.0f));
	//trans = scale(trans, rt::RTVector<float>(0.5f, 0.5f, 0.5f));

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans.getData());

	while(!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// check call events and swap
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return (0);
}
