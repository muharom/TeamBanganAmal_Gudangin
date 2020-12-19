#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture, VBO2, VAO2, EBO2, texture2;
	GLuint VBOLemari, VAOLemari, EBOLemari, textureLemari, VBOFLemari, VAOFLemari, EBOFLemari, textureFLemari;
	GLuint VBO3, VAO3, EBO3, texture3;
	GLuint VBO4, VAO4, EBO4, texture4;
	GLuint VBO5, VAO5, EBO5, texture5;
	GLuint mcVBO, mcVAO, mcEBO, mcTexture;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	void BuildColoredCube();
	void BuildColoredCube2();
	void BuildColoredCube3();
	void BuildMesinCuci();
	void BuildTembok1();
	void BuildColoredPlane();
	void BuildLemari();
	void BuildFLemari();
	void DrawColoredCube();
	void DrawColoredCube2();
	void DrawColoredCube3();
	void DrawMesinCuci();
	void DrawTembok1();
	void DrawColoredPlane();
	void DrawLemari();
	void DrawFLemari();
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};

