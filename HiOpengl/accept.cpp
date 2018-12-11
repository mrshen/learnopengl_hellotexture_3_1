#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "my_shader.h"
#include "stb_image.h"

#include <iostream>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

float alpha_val = 0.2f;

float vertices[] = {
	// positions          // colors           // texture coords (note that we changed them to 'zoom in' on our texture image)
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.f, 1.f, // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.f, 0.f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.f, // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.f, 1.f  // top left 
};

unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

void framebuffer_cb(GLFWwindow*, int, int);
void input_process(GLFWwindow*);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "create window fail!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// process func address with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "glad fail!" << endl;
		glfwTerminate();
		return -1;
	}
	// set frame_buffer callback
	glfwSetFramebufferSizeCallback(window, framebuffer_cb);

	// build and compile our shader program
	// ------------------------------------
	// vertex shader program
	MyShader shaderOrange = MyShader(
		"E:\\VS Projects\\learnopengl_helloshader_2_1\\shaders\\texture_triangle_vertex.vs",
		"E:\\VS Projects\\learnopengl_helloshader_2_1\\shaders\\texture_triangle_fragment.fs"
	);

	// generate textures
	GLuint textures[2];
	glGenTextures(2, textures);


	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char* imageData1 = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (imageData1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Error: FAILED TO LOAD TEXTURE \"container.jpg\"." << endl;
	}
	stbi_image_free(imageData1);


	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	unsigned char* imageData2 = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (imageData2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Error: FAILED TO LOAD TEXTURE awesomeface.png." << endl;
	}
	stbi_image_free(imageData2);
	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	// first triangle data binding
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// clear
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shaderOrange.use();
	shaderOrange.setInt("texture1", 0);
	shaderOrange.setInt("texture2", 1);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		input_process(window);

		glClearColor(0.5f, 0.5f, 0.5f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		shaderOrange.use();
		shaderOrange.setFloat("alpha_val", alpha_val);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// terminate
	glfwTerminate();
	return 0;
}

void framebuffer_cb(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void input_process(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		alpha_val += 0.01f;
		if (alpha_val > 1.f)
		{
			alpha_val = 1.f;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		alpha_val -= 0.01f;
		if (alpha_val < 0.f)
		{
			alpha_val = 0.f;
		}
	}
}