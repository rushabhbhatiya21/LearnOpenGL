#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "shader.h"
#include <stb_image.h>
#include <assert.h>

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpneGL", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

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
	//float firstTriangle[] = {
	//	-0.9f, -0.5f, 0.0f,  // left 
	//	-0.0f, -0.5f, 0.0f,  // right
	//	-0.45f, 0.5f, 0.0f,  // top 
	//};
	//float firstTriangle[] = {
	//	// positions         // colors
	//	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	//	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	//	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	//};
	//float texCoords[] = {
	//	0.0f, 0.0f,  // lower-left corner  
	//	1.0f, 0.0f,  // lower-right corner
	//	0.5f, 1.0f   // top-center corner
	//};
	//float secondTriangle[] = {
	//	// positions         // colors
	//	0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // left
	//	0.9f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // right
	//	0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	//};

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

	//// second triangle setup
	//glBindVertexArray(vao[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // location = 0 in shader
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // location = 1
	//glEnableVertexAttribArray(1);

	//// element buffer
	//GLuint ebo = 0;
	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindVertexArray(0);

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

		// draw with tex
		// ----
		// create texture handle
		unsigned int texture;
		glGenTextures(1, &texture);

		// bind texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// set params
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

		shader1.use();
		GLint currentProgram = 0;

		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "OpenGL error: " << error << std::endl;
		}

		// draw with shader
		// ----
		//shader1.use();
		//glBindVertexArray(vao[0]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.f) + .5f;
		//shader2.use();
		//shader2.set4Float("ourColor", 0.f, greenValue, 0.f, 1.f);
		//shader2.setFloat("offsetX", -.1f);
		//glBindVertexArray(vao[1]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

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
	//shader2.del();

	glfwTerminate();
	return 0;
}