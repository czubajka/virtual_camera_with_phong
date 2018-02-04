#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include <vector>
#include <string>
#include "Vectors.h"
#include <iomanip>

const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void renderSphere();

//tworzenie wektorów kamery
Vector3 cameraPosition{ 0.0f, 0.0f, 3.0f };
Vector3 cameraFront{ 0.0f, 0.0f, -1.0f };
Vector3 cameraUp{ 0.0f, 1.0f, 0.0f };
Vector3 target{ 0.0f, 0.0f, 0.0f };

Vector3 lightPosition{4.2f, 1.0f, 10.0f};
bool sphere = false;
bool lightmove = false;

float deltaTime { 0.0f };	//czas pomiedzy ramkami
float lastFrame { 0.0f };
float lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;
bool firstMouse = true;

float yaw{ -90.0f };
float pitch{ 0.0f };
float fov{ 45.0f };

//material
float shininess{32.0f};
Vector3 ambient{ 0.0215f, 0.1745f, 0.0215f }, diffuse{ 0.0757f, 0.6142f, 0.0757f }, specular{ 0.633f, 0.7278f, 0.633f };

int main()
{
	//inicjalizacja okna
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow * window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Wirtualna Kamera by Karolina Nedza-Sikoniowska", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Nie udalo sie stworzyc okna GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//przechwycenie i obsluga  myszki
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//inicjalizacja GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Nie udala sie inicjalizacja GLAD" << std::endl;
		return -1;
	}
	
	//tworzenie i kompilacja shadera
	Shader shader("res/shader.vs", "res/shader.fs");
	Shader lamp("res/lamp.vs", "res/lamp.fs");

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//tworzeniu buforow i obiektow
	float objects[] = {	
			//position					//normal vectors	
			-0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,

			 -0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,
			 -0.5f,  0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,
			 -0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,
			 -0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,
			 -0.5f, -0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,
			 -0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,
		   	 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f
	};

	Vector3 positions[] = {
		Vector3(0.0f,  0.0f,  0.0f),
		Vector3(2.0f,  5.0f, -15.0f),
		Vector3(-1.5f, -2.2f, -2.5f),
		Vector3(-3.8f, -2.0f, -12.3f),
		Vector3(2.4f, -0.4f, -3.5f),
		Vector3(-1.7f,  3.0f, -7.5f),
		Vector3(1.3f, -2.0f, -2.5f),
		Vector3(1.5f,  2.0f, -2.5f),
		Vector3(1.5f,  0.2f, -1.5f),
		Vector3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO;		//Vetrex Buffer Object ID
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(objects), objects, GL_STATIC_DRAW);

	//tworzenie VERTEX ARRAY OBJECT, przechowywujacy atrybutyj kolejnych VBO (attribute pointer 0, 1, ..., stride itp)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	

	//informowanie openGl jak powinien traktowaæ dane wierzcho³ów
	//Argumenty:
	//1- ktory atribut danych konfigurujemy (layout (location = 0)),
	//2 - wielkosc atrybutu (vec3 = 3),
	//3 - typ danych
	//4 - czy normalizowac dane,
	//5 - odleglosc miedzy tym samym atrybutem kolejnych danych, tzw. stride
	//6 - przesuniecie danych pozycji (offset), typu *void - dlatego wymaga rzutowania
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);						//pozycja
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));		//normalne
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	
									// pêtla renderingu
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		if (lightmove)
		{ 
			lightPosition.x = 1.0f + sin(glfwGetTime()) * 5.0f;
			lightPosition.y = sin(glfwGetTime() / 2.0f) * 5.0f;
		};
		
		//wychwytywanie zdarzen wejscia
		processInput(window);

		//komendy renderingu:
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		Matrix4 projection, view, model;
		
		//tworzenie macierzy definiujacej punkt, w ktory patrzy kamera

		target = cameraPosition + cameraFront;
		view = view.camlookAt(cameraPosition, target, cameraUp);
		
		shader.use();
		unsigned int ambientLocation = glGetUniformLocation(shader.ID, "material.ambient");
		glUniform3f(ambientLocation, ambient.x, ambient.y, ambient.z);

		unsigned int diffuseLocation = glGetUniformLocation(shader.ID, "material.diffuse");
		glUniform3f(diffuseLocation, diffuse.x, diffuse.y, diffuse.z);

		unsigned int specularLocation = glGetUniformLocation(shader.ID, "material.specular");
		glUniform3f(specularLocation, specular.x, specular.y, specular.z);

		unsigned int shininessLocation = glGetUniformLocation(shader.ID, "material.shininess");
		glUniform1f(shininessLocation, shininess);

		unsigned int ambientLightLocation = glGetUniformLocation(shader.ID, "light.ambient");
		glUniform3f(ambientLightLocation, 1.0f, 1.0f, 1.0f);

		unsigned int diffuseLightLocation = glGetUniformLocation(shader.ID, "light.diffuse");
		glUniform3f(diffuseLightLocation, 1.0f, 1.0f, 1.0f);

		unsigned int specularLightLocation = glGetUniformLocation(shader.ID, "light.specular");
		glUniform3f(specularLightLocation, 1.0f, 1.0f, 1.0f);

		unsigned int lightPositionLocation = glGetUniformLocation(shader.ID, "light.position");
		glUniform3f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z);

		unsigned int cameraPositionLocation = glGetUniformLocation(shader.ID, "cameraPosition");
		glUniform3f(cameraPositionLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		unsigned int viewLocation = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view.get());
		
		projection = projection.perspective(fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		unsigned int projectionLocation = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection.get());



		for (int i = 0; i < 10; i++)
		{
			if (sphere) renderSphere();
			float angle = 20.0f * i;
			Matrix4 model;
			model = model.translate(positions[i]);
			model = model.rotate(angle, 0.0f, 1.0f, 0.0f);
			model = model.rotate(angle, 1.0f, 0.0f, 0.0f);

			unsigned int modelLocation = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.get());
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);	
		}
		
		lamp.use();

		projectionLocation = glGetUniformLocation(lamp.ID, "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection.get());
		viewLocation = glGetUniformLocation(lamp.ID, "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view.get());
		model = model.translate(lightPosition);
		model = model.scale(0.2f);

		unsigned int modelLocation = glGetUniformLocation(lamp.ID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.get());

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// wyswietlanie obecnych parametrow
		std::system("cls");
		std::cout << std::fixed << std::setprecision(6);
		std::cout << "\nAmbient vector:" << std::endl;
		std::cout << std::setw(15) << "\t" << ambient.x << "\t" << ambient.y << "\t" << ambient.z << std::endl;
		std::cout << "\nDiffuse vector:" << std::endl;
		std::cout << std::setw(15) << "\t" << diffuse.x << "\t" << diffuse.y << "\t" << diffuse.z << std::endl;
		std::cout << "\nSpecular vector:" << std::endl;
		std::cout << std::setw(15) << "\t" << specular.x << "\t" << specular.y << "\t" << specular.z << std::endl;
		std::cout << "\nShininess:" << std::endl;
		std::cout << std::fixed << std::setprecision(0);
		std::cout << "\t" << shininess << std::endl;

		// zamiana buforow i wychwycanie zdarzen
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &lightVAO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
	float cameraSpeed = 1.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			sphere = true;
		else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
			sphere = false;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPosition += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPosition -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Vector3 cameraRight = cameraFront;
		cameraRight = cameraRight.cross(cameraUp);
		cameraPosition -= cameraRight.normalize() * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Vector3 cameraRight = cameraFront;
		cameraRight = cameraRight.cross(cameraUp);
		cameraPosition += cameraRight.normalize() * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (fov >= 1.0 && fov <= 45.0)
			fov -= 1;
		if (fov <= 1.0)
			fov = 1.0;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (fov >= 1.0 && fov <= 45.0)
			fov += 1;
		if (fov >= 45.0)
			fov = 45.0;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			lightmove = true;
		else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
			lightmove = false;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		{ 
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) shininess += 0.2f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) shininess -= 0.2f;
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) ambient.x += 0.0005f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) ambient.x -= 0.0005f;
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) ambient.y += 0.0005f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) ambient.y -= 0.0005f;
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) ambient.z += 0.0005f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) ambient.z -= 0.0005f;
		}
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) diffuse.x += 0.0005f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) diffuse.x -= 0.0005f;
		}
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) diffuse.y += 0.0005f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) diffuse.y -= 0.0005f;
		}
		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) diffuse.z += 0.0005f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) diffuse.z -= 0.0005f;
		}
		if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) specular.x += 0.005f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) specular.x -= 0.005f;
		}
		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) specular.y += 0.005f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) specular.y -= 0.005f;
		}
		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) specular.z += 0.005f;
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) specular.z -= 0.005f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	Vector3 front;
	front.x = cos(yaw*DEG2RAD) * cos(pitch*DEG2RAD);
	front.y = sin(pitch*DEG2RAD);
	front.z = sin(yaw*DEG2RAD) * cos(pitch*DEG2RAD);
	cameraFront = front.normalize();
};


// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<Vector3> positions;
		std::vector<Vector3> uv;
		std::vector<Vector3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(Vector3(xPos, yPos, zPos));
				uv.push_back(Vector3(xSegment, ySegment, 0.0f));
				normals.push_back(Vector3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		std::vector<float> data;
		for (int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				//data.push_back(uv[i].x);
				//data.push_back(uv[i].y);
			}
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

