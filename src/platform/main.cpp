#pragma region headers

#pragma region in-built
#include <iostream>
#include <algorithm>
#include <assert.h>
#pragma endregion

#pragma region platform
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma endregion

#pragma region custom
#include "shader.h"
#pragma endregion

#pragma endregion

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// stores how much we're seeing of either texture
float mixValue = 0.2f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpneGL", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum no of vertex attributes supported: " << nrAttributes << std::endl;

	// build and compile our shader program
	// ------------------------------------
	Shader shader1(RESOURCES_PATH "shaders/shader.vs", RESOURCES_PATH "shaders/shader1.fs");
	std::cout << "Shader1 ID: " << shader1.ID << std::endl;
	//Shader shader2(RESOURCES_PATH "shaders/shader.vs", RESOURCES_PATH "shaders/shader2.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
		0, 3, 2    // second triangle
	};

	std::cout << sizeof(vertices) << std::endl;

	// vertex buffers & vertex arrays
	GLuint vbo[2], vao[2];
	glGenBuffers(2, vbo);
	glGenVertexArrays(2, vao);

	// first triangle setup
	// location = 0, vec3, float, already normalized, spacing bw each vertex attribute (stride), offset
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), static_cast<void*>(vertices), GL_STATIC_DRAW);

	GLint size = 0;

	glGetBufferParameteriv(
		GL_ARRAY_BUFFER,
		GL_BUFFER_SIZE,
		&size
	);

	std::cout << "VBO size: " << size << std::endl;

	// stride = single vertex size (bytes), void *pointer = offset of each param inside vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // location = 0 in shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // location = 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // location = 2
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// create texture handle
	unsigned int texture, texture2;
	glGenTextures(1, &texture);

	// by default 0 is activated
	// can use at least a minimum of 16 texture units
	glActiveTexture(GL_TEXTURE0);
	// bind texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// set params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(RESOURCES_PATH "container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		// create texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// create mipmaps for texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::IMAGE::FAILED TO LOAD IMAGE FROM FILE" << std::endl;
	}
	assert(data);

	// free image
	stbi_image_free(data);

	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load(RESOURCES_PATH "awesomeface.png", &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::IMAGE::FAILED TO LOAD IMAGE FROM FILE" << std::endl;
	}
	assert(data);

	stbi_image_free(data);

	shader1.use();
	shader1.setInt("texture1", 0);
	shader1.setInt("texture2", 1);
	shader1.setFloat("mixValue", mixValue);

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.f); // clear color
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// create transformations
		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// get matrix's uniform location and set matrix
		shader1.use();
		unsigned int transformLocation = glGetUniformLocation(shader1.ID, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(vao[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "OpenGL error: " << error << std::endl;
		}

		// scalar triangle
		transform = glm::mat4(1.f);
		transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 1.f));
		float scalar = static_cast<float>(glm::sin(glfwGetTime()));
		transform = glm::scale(transform, glm::vec3(scalar, scalar, 1.f));

		// draw scalar triangle
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value, both are valid
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
	shader1.del();

	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mixValue = std::clamp(mixValue + 0.01f, 0.f, 1.f);

	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mixValue = std::clamp(mixValue - 0.01f, 0.f, 1.f);
}