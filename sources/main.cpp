#define GL_SILENCE_DEPRECATION
#include <glad/glad.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <limits>
#include "texture_loader.hpp"
#include "rt_vector.hpp"
#include "rt_matrix.hpp"
#include "Vector.hpp"
#include "Pair.hpp"

rt::RTMatrix<float> translate(
	rt::RTMatrix<float> matrix,
	rt::RTVector<float> transVector
) {
	rt::RTMatrix<float> identityMatrix(4, 4);
	identityMatrix.toIdentity();

	identityMatrix[0][3] = transVector['x'];
	identityMatrix[1][3] = transVector['y'];
	identityMatrix[2][3] = transVector['z'];

	return matrix * identityMatrix;
}

rt::RTMatrix<float> rotate(
	rt::RTMatrix<float> matrix,
	float rad,
	rt::RTVector<float> rotAxis
) {
	rt::RTMatrix<float> iMat(4, 4);
	iMat.toIdentity();

	iMat[0][0] = cos(rad) + rotAxis['x'] * rotAxis['x'] * (1 - cos(rad));
	iMat[1][0] = rotAxis['y'] * rotAxis['x'] * (1 - cos(rad)) + rotAxis['z'] * sin(rad);
	iMat[2][0] = rotAxis['z'] * rotAxis['x'] * (1 - cos(rad)) - rotAxis['y'] * sin(rad);

	iMat[0][1] = rotAxis['x'] * rotAxis['y'] * (1 - cos(rad)) - rotAxis['z'] * sin(rad);
	iMat[1][1] = cos(rad) + rotAxis['y'] * rotAxis['y'] * (1 - cos(rad));
	iMat[2][1] = rotAxis['z'] * rotAxis['y'] * (1 - cos(rad)) + rotAxis['x'] * sin(rad);

	iMat[0][2] = rotAxis['x'] * rotAxis['z'] * (1 - cos(rad)) + rotAxis['y'] * sin(rad);
	iMat[1][2] = rotAxis['y'] * rotAxis['z'] * (1 - cos(rad)) - rotAxis['x'] * sin(rad);
	iMat[2][2] = cos(rad) + rotAxis['z'] * rotAxis['z'] * (1 - cos(rad));

	return matrix * iMat;
}

rt::RTMatrix<float> scale(
	rt::RTMatrix<float> matrix,
	rt::RTVector<float> scaleVector
) {
	rt::RTMatrix<float> identityMatrix(4, 4);
	identityMatrix.toIdentity();

	identityMatrix[0][0] = scaleVector['x'];
	identityMatrix[1][1] = scaleVector['y'];
	identityMatrix[2][2] = scaleVector['z'];
	return matrix * identityMatrix;
}

float radians(float angle) {
	return angle * M_PI / 180;
}

rt::RTMatrix<float> perspective(
	float fov,
	float aspect,
	float near,
	float far
) {
	rt::RTMatrix<float> perspectiveMatrix(4, 4);

	float tan_half_angle = tan(fov / 2);

	perspectiveMatrix[0][0] = 1 / (aspect * tan_half_angle);
	perspectiveMatrix[1][1] = 1 / tan_half_angle;
	perspectiveMatrix[2][2] = -(far + near) / (far - near);
	perspectiveMatrix[2][3] = -(2 * far * near) / (far - near);
	perspectiveMatrix[3][2] = -1;

	return perspectiveMatrix;
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	long long int	num;
	int				minus;

	minus = 1;
	while (*str == '\f' || *str == '\v' || *str == ' '
		|| *str == '\t' || *str == '\n' || *str == '\r')
		str++;
	if (*str == '-')
	{
		minus = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	num = 0;
	while (*str != '\0')
	{
		if (*str < '0' || *str > '9')
			break ;
		num += *str - '0';
		num *= 10;
		str++;
	}
	return (num / 10 * minus);
}

float	ft_atof(const char *str)
{
	int		n;
	float	d;
	int		len;
	int		sign;

	sign = 1;
	if (str[0] == '-' && str[1] == '0')
		sign = -1;
	d = 0;
	len = 0;
	n = ft_atoi(str);
	while (ft_isdigit(*str) || *str == '-')
		str++;
	if (*str == '.')
	{
		str++;
		while (ft_isdigit(str[len]))
			len++;
		d = ft_atoi(str);
	}
	while (len-- > 0)
		d = d / 10;
	if (n < 0)
		d = d * -1;
	return (((double)n + d) * sign);
}

bool isNumber(ft::String str) {
	for (auto it = str.begin(); it != str.end(); it++) {
		if (!ft_isdigit(*it)) {
			return false;
		}
	}
	return true;
}

bool loadOBJ(
	const char *path,
	ft::Vector<float> &outVertices,
	ft::Vector<int> &outVertexIndices,
    rt::RTVector<float> &center
) {
	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		return false;
	}

    rt::RTVector<float> min(
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()
    );
    rt::RTVector<float> max(
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::min()
    );

    ft::String tempStr;
    file >> tempStr;
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
    size_t counter = 0;
	while (file) {
		if (tempStr == "v") {
			file >> tempStr;
            float x = ft_atof(tempStr.c_str());
            outVertices.push_back(x);
            min['x'] = x < min['x'] ? x : min['x'];
            max['x'] = x > max['x'] ? x : max['x'];
			file >> tempStr;
            float y = ft_atof(tempStr.c_str());
            outVertices.push_back(y);
            min['y'] = y < min['y'] ? y : min['y'];
            max['y'] = y > max['y'] ? y : max['y'];
			file >> tempStr;
            float z = ft_atof(tempStr.c_str());
            outVertices.push_back(z);
            min['z'] = z < min['z'] ? z : min['z'];
            max['z'] = z > max['z'] ? z : max['z'];
            file >> tempStr;

            red = sin(counter) / 2.0f + 0.5f;
            green = cos(counter) / 2.0f + 0.5f;
            blue = tan(counter) / 2.0f + 0.5f;
            outVertices.push_back(red);
            outVertices.push_back(green);
            outVertices.push_back(blue);
            counter++;
		} else if (tempStr == "f") {
            ft::Vector<float> tempVector;
			while (file >> tempStr && isNumber(tempStr)) {
                tempVector.push_back(ft_atoi(tempStr.c_str()) - 1);
                if (tempVector.size() == 3) {
                    outVertexIndices.push_back(tempVector[0]);
                    outVertexIndices.push_back(tempVector[1]);
                    outVertexIndices.push_back(tempVector[2]);
                    tempVector.erase(tempVector.begin() + 1);
                }
			}
		} else {
            file >> tempStr;
        }
	}
    center = rt::RTVector(
            (max['x'] - min['x']) / 2.0f,
            (max['y'] - min['y']) / 2.0f,
            (max['z'] - min['z']) / 2.0f
    );
	return true;
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
		"flat out vec4 ourColor;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"	ourColor = vec4(aColor, 1.0);\n"
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
		"flat in vec4 ourColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"   FragColor = ourColor + texture(ourTexture, TexCoord);\n" //
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

	// Scop::TextureLoader textLoader("water.bmp");
	// unsigned char* image = textLoader.getPixelArray();
	// size_t width = textLoader.getWidth();
	// size_t height = textLoader.getHeight();

//// 					Check texture 					/////
	const size_t width = 64;
	const size_t height = 64;
	GLubyte image[width][height][3];
	int i, j, c;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
			image[i][j][0] = (GLubyte) c;
			image[i][j][1] = (GLubyte) c;
			image[i][j][2] = (GLubyte) c;
		}
	}

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

//////////////////////////Load obj///////////////////////////////////////////////

	ft::Vector<float> vertices;
	ft::Vector<int> indices;
    rt::RTVector<float> center;

	loadOBJ("models/42.obj", vertices, indices, center);

//	for (auto it = vertices.begin(); it != vertices.end(); it++) {
//		std::cout << *it << std::endl;
//	}
//	for (auto it = indices.begin(); it != indices.end(); it++) {
//        std::cout << *it << ' ';
//	}
//    std::cout << std::endl;

////////////////////////////////////////////////////////////////////////////////

	// float vertices[] = {
	// 	-0.5f, -0.5f, 0.28867f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	// 	0.5f, -0.5f, 0.28867f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	// 	0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		0.5f, 1.0f,

	// 	0.5f, -0.5f, 0.28867f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	// 	0.0f, -0.5f, -0.577f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	// 	0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		0.5f, 1.0f,

	// 	0.0f, -0.5f, -0.577f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	// 	-0.5f, -0.5f, 0.28867f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	// 	0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		0.5f, 1.0f,

	// 	-0.5f, -0.5f, 0.28867f,	0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	// 	0.5f, -0.5f, 0.28867f,	0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	// 	0.0f, -0.5f, -0.577f,	0.0f, 0.0f, 0.0f,		0.5f, 1.0f,
	// };

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// unsigned int indices[] = {
	// 	0, 1, 2,
	// 	3, 4, 5,
	// 	6, 7, 8,
	// 	9, 10, 11
	// };

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	 glEnableVertexAttribArray(1);

	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// glUseProgram(shaderProgram);
	// glUniform1i(glGetUniformLocation(shaderProgram, "texture"), 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

/////////////// Transformation matrix //////////////////////////////////////////
	float angle = 0;
	glEnable(GL_DEPTH_TEST);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(shaderProgram);

///////////////////// Transformation ///////////////////////////////////////////////////////////////////////////////////
		// Model matrix
		rt::RTMatrix<float> model(4, 4);
		model.toIdentity();
        model = translate(model, rt::RTVector<float>(
                -center['x'],
                -center['y'],
                -center['z']
        ));
		model = scale(model, rt::RTVector<float>(1.0f, 1.0f, 1.0f));
		model = rotate(model, radians(angle), rt::RTVector<float>(0.0f, 1.0f, 0.0f));
		angle += 1;

		//glUseProgram(shaderProgram);
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_TRUE, (model).getData());

		// View matrix
		rt::RTMatrix<float> view(4, 4);
		view.toIdentity();
		view = translate(view, rt::RTVector<float>(0.0f, 0.0f, -10.0f));

		//glUseProgram(shaderProgram);
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, (view).getData());

		rt::RTMatrix<float> projection = perspective(radians(45), 800.0f / 600.0f, 0.1f, 100.0f);

		//glUseProgram(shaderProgram);
		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, (projection).getData());

		///////////////////////////////////////////////////////////////////////////////
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// check call events and swap
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return (0);
}
