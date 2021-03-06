#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 Color;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
	"	Color = aColor;\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 Color;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(Color, 1.0f);\n"
"}\0";

void Translation(int x0, int y0, int x1, int y1);
void Rotation(int x1, int y1, int x2, int y2, int x3, int y3);
void Scaling(int x0, int y0, int r);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG_HW4", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	float vertices[] = {
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 0.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  0.0f, 0.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  0.0f, 0.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 0.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 0.0f,

		-2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		-2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 0.0f,

		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 0.0f,

		 2.0f,  2.0f,  2.0f,  0.0f, 0.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  0.0f, 0.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f, 0.0f, 1.0f,

		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,

		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 1.0f,
	};
	// world space positions of our cubes
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 color;

	bool orthographic = true;
	bool perspective = false;
	bool viewchanging = false;


	while (!glfwWindowShouldClose(window))
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glfwPollEvents();

		static float x = -1.5f;
		static float y = 0.5f;
		static float z = -1.5f;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Color Setting", NULL, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Mode"))
			{
				if (ImGui::MenuItem("Orthographic", "Ctrl+O")) { 
					orthographic = true;
					perspective = false;
					viewchanging = false;
					x = -1.5f;
					y = 0.5f;
					z = -1.5f;
				}
				if (ImGui::MenuItem("Perspective", "Ctrl+P")) { 
					orthographic = false;
					perspective = true;
					viewchanging = false;
					x = -1.5f;
					y = 0.5f;
					z = -1.5f;
				}
				if (ImGui::MenuItem("Viewchanging", "Ctrl+V")) {
					orthographic = false;
					perspective = false;
					viewchanging = true;
					x = 0;
					y = 0;
					z = 0;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
		

		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);


		if (!viewchanging) {
			if (orthographic) {
				static float left = -8.0f;
				static float right = 8.0f;
				static float bottom = -6.0f;
				static float top = 6.0f;
				static float nearp = 0.1f;
				static float farp = 100.0f;
				ImGui::Begin("Parameter Setting");
				ImGui::SliderFloat("left", &left, -16, 16);
				ImGui::SliderFloat("right", &right, -16, 16);
				ImGui::SliderFloat("bottom", &bottom, -12, 12);
				ImGui::SliderFloat("top", &top, -12, 12);
				ImGui::SliderFloat("near", &nearp, 0, 10);
				ImGui::SliderFloat("far", &farp, 0, 24);
				ImGui::End();
				glm::mat4 projection;
				//projection = glm::ortho(-8.0f, 8.0f, -6.0f, 6.0f, 0.1f, 100.0f);
				projection = glm::ortho(left, right, bottom, top, nearp, farp);
				glUniformMatrix4fv(glGetUniformLocation(shaderProgram, std::string("projection").c_str()), 1, GL_FALSE, &projection[0][0]);
			}
			else if (perspective) {
				static float fov = 45.0f;
				static float ratio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
				static float nearp = 0.1f;
				static float farp = 100.0f;
				ImGui::Begin("Parameter Setting");
				ImGui::SliderFloat("fov", &fov, 0, 360);
				ImGui::SliderFloat("ratio", &ratio, 0, 10);
				ImGui::SliderFloat("near", &nearp, 0, 10);
				ImGui::SliderFloat("far", &farp, 0, 24);
				ImGui::End();
				glm::mat4 projection;
				projection = glm::perspective(glm::radians(fov), ratio, nearp, farp);
				glUniformMatrix4fv(glGetUniformLocation(shaderProgram, std::string("projection").c_str()), 1, GL_FALSE, &projection[0][0]);
			}
			glm::mat4 view;
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));
			unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		}
		else {
			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, std::string("projection").c_str()), 1, GL_FALSE, &projection[0][0]);

			float radius = 10.0f;
			float camPosX = sin(glfwGetTime()) * radius;
			float camPosZ = cos(glfwGetTime()) * radius;
			glm::mat4 view;
			view = glm::lookAt(glm::vec3(camPosX, 0.0f, camPosZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		}

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(x, y, z));
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);

	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

void Translation(int x0, int y0, int x1, int y1) {


	//glBufferData(GL_ARRAY_BUFFER, 4 * 3 * dx, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glDrawArrays(GL_POINTS, 0, dx);

}


void Rotation(int x1, int y1, int x2, int y2, int x3, int y3) {

}

void Scaling(int x0, int y0, int r) {


	//glBufferData(GL_ARRAY_BUFFER, 4 * i, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glDrawArrays(GL_POINTS, 0, i/3);
}