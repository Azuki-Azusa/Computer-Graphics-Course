#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

void drawLine(int x0, int y0, int x1, int y1);
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
void drawCircle(int x0, int y0, int r);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "CG_HW3", NULL, NULL);
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




	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



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

	bool triangleWin = true;
	bool circleWin = false;


	while (!glfwWindowShouldClose(window))
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Color Setting", NULL, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Mode"))
			{
				if (ImGui::MenuItem("triangle", "Ctrl+T")) { 
					triangleWin = true;
					circleWin = false;
				}
				if (ImGui::MenuItem("circle", "Ctrl+R")) { 
					circleWin = true; 
					triangleWin = false;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
		

		processInput(window);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		if (triangleWin) {
			static int x0 = -150;
			static int y0 = -150;
			static int x1 = 0;
			static int y1 = 150;
			static int x2 = 150;
			static int y2 = -150;
			ImGui::Begin("Setting radius");
			ImGui::SliderInt("x0", &x0, -300, 300);
			ImGui::SliderInt("y0", &y0, -300, 300);
			ImGui::SliderInt("x1", &x1, -300, 300);
			ImGui::SliderInt("y1", &y1, -300, 300);
			ImGui::SliderInt("x2", &x2, -300, 300);
			ImGui::SliderInt("y2", &y2, -300, 300);
			drawTriangle(x0, y0, x1, y1, x2, y2);
			ImGui::End();
		}
		else if (circleWin) {
			static int r = 0;
			ImGui::Begin("Setting radius");
			ImGui::SliderInt("pixel", &r, 0, 300);
			drawCircle(0, 0, r);
			ImGui::End();
		}
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

void drawLine(int x0, int y0, int x1, int y1) {
	float vertices[1803] = { 0 };
	int i = 0;
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	bool ykey = y1 > y0 ? 1 : 0;
	bool kkey = abs(y1 - y0) > abs(x1 - x0) ? 1 : 0;
	y0 = ykey ? y0 : -y0;
	y1 = ykey ? y1 : -y1;
	if (kkey) {
		swap(x0, y0);
		swap(x1, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int ddy = 2 * dy;
	int ddy_ddx = 2 * dy - 2 * dx;
	int p = 2 * dy - dx;

	//std::cout << x0 << y0 << x1 << y1 << std::endl;
	if (kkey) {
		vertices[i++] = (float)y0 / 300;
		vertices[i++] = (float)(ykey ? x0 : -x0) / 300;
	}
	else {
		vertices[i++] = (float)x0 / 300;
		vertices[i++] = (float)(ykey ? y0 : -y0) / 300;
	}
	i++;

	for (int x = x0, y = y0; x < x1;) {
		if (p <= 0) {
			x++;
			if (kkey) {
				vertices[i++] = (float)y / 300;
				vertices[i++] = (float)(ykey ? x : -x) / 300;
			}
			else {
				vertices[i++] = (float)x / 300;
				vertices[i++] = (float)(ykey ? y : -y) / 300;
			}
			p = p + ddy;
		}
		else {
			x++;
			y++;
			if (kkey) {
				vertices[i++] = (float)y / 300;
				vertices[i++] = (float)(ykey ? x : -x) / 300;
			}
			else {
				vertices[i++] = (float)x / 300;
				vertices[i++] = (float)(ykey ? y : -y) / 300;
			}
			p = p + ddy_ddx;
		}
		i++;
		//std::cout << x << " " << y << std::endl;
	}
	/*
	for (int j = 0; j < dx;) {
		std::cout << vertices[j] << " " << vertices[j + 1] << std::endl;
		j += 3;
	}
	*/

	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * dx, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_POINTS, 0, dx);

}


void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	drawLine(x1, y1, x2, y2);
	drawLine(x2, y2, x3, y3);
	drawLine(x3, y3, x1, y1);
}

void drawCircle(int x0, int y0, int r) {
	float vertices[300 * 8 * 3] = { 0 };
	int p = 3 - r * 2;
	int i = 0;
	for (int x = 0, y = r; x <= y; x++) {
		{
			vertices[i++] = (float)x / 300;
			vertices[i++] = (float)y / 300;
			i++;
			vertices[i++] = (float)-x / 300;
			vertices[i++] = (float)-y / 300;
			i++;
			vertices[i++] = (float)y / 300;
			vertices[i++] = (float)x / 300;
			i++;
			vertices[i++] = (float)-y / 300;
			vertices[i++] = (float)-x / 300;
			i++;
			vertices[i++] = (float)x / 300;
			vertices[i++] = (float)-y / 300;
			i++;
			vertices[i++] = (float)-x / 300;
			vertices[i++] = (float)y / 300;
			i++;
			vertices[i++] = (float)y / 300;
			vertices[i++] = (float)-x / 300;
			i++;
			vertices[i++] = (float)-y / 300;
			vertices[i++] = (float)x / 300;
			i++;
		}
		if (p >= 0) {
			p += 4 * (x - y) + 10;
			y--;
		}
		else p += 4 * x + 6;
	}

	glBufferData(GL_ARRAY_BUFFER, 4 * i, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_POINTS, 0, i/3);
}