#include "Demo.h"

Demo::Demo() {

}

Demo::~Demo() {
}

void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();
	BuildColoredCube2();
	BuildColoredCube3();

	BuildMesinCuci();

	BuildTembok1();

	BuildColoredPlane();

	BuildLemari();

	BuildFLemari();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth/2;
	double midY = screenHeight/2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);


	

}

void Demo::Update(double deltaTime) {
	
}
void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// set lighting attribute
	GLint lightPosLoc = glGetUniformLocation(this->shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 0, 1, 0);
	GLint viewPosLoc = glGetUniformLocation(this->shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0, 2, 3);
	GLint lightColorLoc = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

	DrawColoredCube();
	DrawColoredCube2();
	DrawColoredCube3();
	DrawMesinCuci();
	DrawTembok1();
	DrawColoredPlane();
	DrawLemari();
	DrawFLemari();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kerdus1.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		// front
		-4.5, -0.5, -2.7, 0, 0,		0.0f,  0.0f,  1.0f,	// 0
		-3.5, -0.5, -2.7, 1, 0,		0.0f,  0.0f,  1.0f,  // 1
		-3.5,  0.5, -2.7, 1, 1,		0.0f,  0.0f,  1.0f,  // 2
		-4.5,  0.5, -2.7, 0, 1,		0.0f,  0.0f,  1.0f,	// 3

		// right	   
		-3.5,  0.5, -2.7, 0, 0,		1.0f,  0.0f,  0.0f,  // 4
		-3.5,  0.5, -3.7, 1, 0,		1.0f,  0.0f,  0.0f,  // 5
		-3.5, -0.5, -3.7, 1, 1,		1.0f,  0.0f,  0.0f,  // 6
		-3.5, -0.5, -2.7, 0, 1,		1.0f,  0.0f,  0.0f,  // 7
					   
		// back		   
		-4.5, -0.5, -3.7, 0, 0,		0.0f,  0.0f,  -1.0f, // 8 
		-3.5, -0.5, -3.7, 1, 0,		0.0f,  0.0f,  -1.0f, // 9
		-3.5,  0.5, -3.7, 1, 1,		0.0f,  0.0f,  -1.0f, // 10
		-4.5,  0.5, -3.7, 0, 1,		0.0f,  0.0f,  -1.0f, // 11
					   
		// left		   
		-4.5, -0.5, -3.7, 0, 0,		-1.0f,  0.0f,  0.0f, // 12
		-4.5, -0.5, -2.7, 1, 0,		-1.0f,  0.0f,  0.0f, // 13
		-4.5,  0.5, -2.7, 1, 1,		-1.0f,  0.0f,  0.0f, // 14
		-4.5,  0.5, -3.7, 0, 1,		-1.0f,  0.0f,  0.0f, // 15
					   
		// upper			    
		-3.5,  0.5, -2.7, 0, 0,		0.0f,  1.0f,  0.0f, // 16
		-4.5,  0.5, -2.7, 1, 0,		0.0f,  1.0f,  0.0f, // 17
		-4.5,  0.5, -3.7, 1, 1,		0.0f,  1.0f,  0.0f, // 18
		-3.5,  0.5, -3.7, 0, 1,		0.0f,  1.0f,  0.0f, // 19
					   
		// bottom	   
		-4.5, -0.5, -3.7, 0, 0,		0.0f,  -1.0f,  0.0f, // 20
		-3.5, -0.5, -3.7, 1, 0,		0.0f,  -1.0f,  0.0f, // 21
		-3.5, -0.5, -2.7, 1, 1,		0.0f,  -1.0f,  0.0f, // 22
		-4.5, -0.5, -2.7, 0, 1,		0.0f,  -1.0f,  0.0f, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::BuildColoredCube2() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kerdus1.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		// front
		-4.5, -0.5, -1.6, 0, 0,		0.0f,  0.0f,  1.0f,	// 0
		-3.5, -0.5, -1.6, 1, 0,		0.0f,  0.0f,  1.0f,  // 1
		-3.5,  0.5, -1.6, 1, 1,		0.0f,  0.0f,  1.0f,  // 2
		-4.5,  0.5, -1.6, 0, 1,		0.0f,  0.0f,  1.0f,	// 3
					   
		// right	   
		-3.5,  0.5, -1.6, 0, 0,		1.0f,  0.0f,  0.0f,  // 4
		-3.5,  0.5, -2.6, 1, 0,		1.0f,  0.0f,  0.0f,  // 5
		-3.5, -0.5, -2.6, 1, 1,		1.0f,  0.0f,  0.0f,  // 6
		-3.5, -0.5, -1.6, 0, 1,		1.0f,  0.0f,  0.0f,  // 7
					   
		// back		   
		-4.5, -0.5, -2.6, 0, 0,		0.0f,  0.0f,  -1.0f, // 8 
		-3.5, -0.5, -2.6, 1, 0,		0.0f,  0.0f,  -1.0f, // 9
		-3.5,  0.5, -2.6, 1, 1,		0.0f,  0.0f,  -1.0f, // 10
		-4.5,  0.5, -2.6, 0, 1,		0.0f,  0.0f,  -1.0f, // 11
					   
		// left		   
		-4.5, -0.5, -2.6, 0, 0,		-1.0f,  0.0f,  0.0f, // 12
		-4.5, -0.5, -1.6, 1, 0,		-1.0f,  0.0f,  0.0f, // 13
		-4.5,  0.5, -1.6, 1, 1,		-1.0f,  0.0f,  0.0f, // 14
		-4.5,  0.5, -2.6, 0, 1,		-1.0f,  0.0f,  0.0f, // 15

		// upper			    
		-3.5,  0.5, -1.6, 0, 0,		0.0f,  1.0f,  0.0f, // 16
		-4.5,  0.5, -1.6, 1, 0,		0.0f,  1.0f,  0.0f, // 17
		-4.5,  0.5, -2.6, 1, 1,		0.0f,  1.0f,  0.0f, // 18
		-3.5,  0.5, -2.6, 0, 1,		0.0f,  1.0f,  0.0f, // 19

		// bottom
		-4.5, -0.5, -2.6, 0, 0,		0.0f,  -1.0f,  0.0f, // 20
		-3.5, -0.5, -2.6, 1, 0,		0.0f,  -1.0f,  0.0f, // 21
		-3.5, -0.5, -1.6, 1, 1,		0.0f,  -1.0f,  0.0f, // 22
		-4.5, -0.5, -1.6, 0, 1,		0.0f,  -1.0f,  0.0f, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::BuildColoredCube3() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kerdus1.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		// front
		-4.5, -0.5, -0.5, 0, 0,		0.0f,  0.0f,  1.0f,	// 0
		-3.5, -0.5, -0.5, 1, 0,		0.0f,  0.0f,  1.0f,  // 1
		-3.5,  0.5, -0.5, 1, 1,		0.0f,  0.0f,  1.0f,  // 2
		-4.5,  0.5, -0.5, 0, 1,		0.0f,  0.0f,  1.0f,	// 3

		// right
		-3.5,  0.5, -0.5, 0, 0,		1.0f,  0.0f,  0.0f,  // 4
		-3.5,  0.5, -1.5, 1, 0,		1.0f,  0.0f,  0.0f,  // 5
		-3.5, -0.5, -1.5, 1, 1,		1.0f,  0.0f,  0.0f,  // 6
		-3.5, -0.5, -0.5, 0, 1,		1.0f,  0.0f,  0.0f,  // 7

		// back
		-4.5, -0.5, -1.5, 0, 0,		0.0f,  0.0f,  -1.0f, // 8 
		-3.5, -0.5, -1.5, 1, 0,		0.0f,  0.0f,  -1.0f, // 9
		-3.5,  0.5, -1.5, 1, 1,		0.0f,  0.0f,  -1.0f, // 10
		-4.5,  0.5, -1.5, 0, 1,		0.0f,  0.0f,  -1.0f, // 11

		// left
		-4.5, -0.5, -1.5, 0, 0,		-1.0f,  0.0f,  0.0f, // 12
		-4.5, -0.5, -0.5, 1, 0,		-1.0f,  0.0f,  0.0f, // 13
		-4.5,  0.5, -0.5, 1, 1,		-1.0f,  0.0f,  0.0f, // 14
		-4.5,  0.5, -1.5, 0, 1,		-1.0f,  0.0f,  0.0f, // 15

		// upper			    
		-3.5, 0.5, -0.5, 0, 0,		0.0f,  1.0f,  0.0f, // 16
		-4.5, 0.5, -0.5, 1, 0,		0.0f,  1.0f,  0.0f, // 17
		-4.5, 0.5, -1.5, 1, 1,		0.0f,  1.0f,  0.0f, // 18
		-3.5, 0.5, -1.5, 0, 1,		0.0f,  1.0f,  0.0f, // 19

		// bottom
		-4.5, -0.5, -1.5, 0, 0,		0.0f,  -1.0f,  0.0f, // 20
		-3.5, -0.5, -1.5, 1, 0,		0.0f,  -1.0f,  0.0f, // 21
		-3.5, -0.5, -0.5, 1, 1,		0.0f,  -1.0f,  0.0f, // 22
		-4.5, -0.5, -0.5, 0, 1,		0.0f,  -1.0f,  0.0f, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::BuildMesinCuci() {
 glGenTextures(1, &mcTexture);
 glBindTexture(GL_TEXTURE_2D, mcTexture);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 int width, height;
 unsigned char* image = SOIL_load_image("mesincuci.png", &width, &height, 0, SOIL_LOAD_RGBA);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
 SOIL_free_image_data(image);
 glBindTexture(GL_TEXTURE_2D, 0);
 float vertices[] = {
  // format position, tex coords, normal
  // front
  -4.9, -0.5,  4.9,		0,   0,			 0.0f,  0.0f,  1.0f,  // 0          
  -3.7, -0.5,  4.9,		0.5, 0,			 0.0f,  0.0f,  1.0f,  // 1
  -3.7,  1.3,  4.9,		0.1, 0.1,		 0.0f,  0.0f,  1.0f,  // 2
  -4.9,  1.3,  4.9,		0.1, 0.1,		 0.0f,  0.0f,  1.0f,  // 3
														  
  // right												  
  -3.7, -0.5,  4.9,		0, 0,			 1.0f,  0.0f,  0.0f,  // 4
  -3.7, -0.5,  3.4,		1, 0,			 1.0f,  0.0f,  0.0f,  // 5
  -3.7,  1.3,  3.4,		1, 1,			 1.0f,  0.0f,  0.0f,  // 6
  -3.7,  1.3,  4.9,		0, 1,			 1.0f,  0.0f,  0.0f,  // 7
														  
  // back												  
  -4.9, -0.5,  3.4,		0,   0,			 0.0f,  0.0f,  -1.0f, // 8 
  -3.7, -0.5,  3.4,		0.5, 0,			 0.0f,  0.0f,  -1.0f, // 9
  -3.7,  1.3,  3.4,		0.1, 0.1,		 0.0f,  0.0f,  -1.0f, // 10
  -4.9,  1.3,  3.4,		0.1, 0.1,		 0.0f,  0.0f,  -1.0f, // 11
														  
  // left   4											  
  -4.9, -0.5,  3.4,		0,   0,			-1.0f,  0.0f,  0.0f,  // 12
  -4.9, -0.5,  4.9,		0.5, 0,			-1.0f,  0.0f,  0.0f,  // 13
  -4.9,  1.3,  4.9,		0.1, 0.1,		-1.0f,  0.0f,  0.0f,  // 14
  -4.9,  1.3,  3.4,		0.1, 0.1,		-1.0f,  0.0f,  0.0f,  // 15
														  
  // upper4												  
  -4.9,  1.3,  4.9,		0,   0,			 0.0f,  1.0f,  0.0f,  // 16
  -3.7,  1.3,  4.9,		0.5, 0,			 0.0f,  1.0f,  0.0f,  // 17
  -3.7,  1.3,  3.4,		0.1, 0.1,		 0.0f,  1.0f,  0.0f,  // 18
  -4.9,  1.3,  3.4,		0.1, 0.1,		 0.0f,  1.0f,  0.0f,  // 19
														  
  // bottom												  
  -4.9, -0.5,  4.9,		0,   0,			 0.0f,  -1.0f,  0.0f, // 20
  -3.7, -0.5,  4.9,		0.5, 0,			 0.0f,  -1.0f,  0.0f, // 21
  -3.7, -0.5,  3.4,		0.1, 0.1,		 0.0f,  -1.0f,  0.0f, // 22
  -4.9, -0.5,  3.4,		0.1, 0.1,		 0.0f,  -1.0f,  0.0f, // 23
 };

 unsigned int indices[] = {
0,  1,  2,  0,  2,  3,   // front
4,  5,  6,  4,  6, 7,   // right
8,  9,  10, 8,  10, 11,  // back
12, 14, 13, 12, 15, 14,  // left
16, 18, 17, 16, 19, 18,  // upper
20, 22, 21, 20, 23, 22   // bottom
 };

 glGenVertexArrays(1, &mcVAO);
 glGenBuffers(1, &mcVBO);
 glGenBuffers(1, &mcEBO);
 // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
 glBindVertexArray(mcVAO);

 glBindBuffer(GL_ARRAY_BUFFER, mcVBO);
 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mcEBO);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

 // define position pointer layout 0
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
 glEnableVertexAttribArray(0);

 // define texcoord pointer layout 1
 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
 glEnableVertexAttribArray(1);

 // define normal pointer layout 2
 glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
 glEnableVertexAttribArray(2);

 // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
 glBindBuffer(GL_ARRAY_BUFFER, 0);

 // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
 // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
 glBindVertexArray(0);

 // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::BuildTembok1() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	int width, height;
	unsigned char* image = SOIL_load_image("tembokBata.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-5.0, -5.0, 5.0, 0, 0,		 0.0f,  0.0f,  0.0f,	// 0
		 5.0, -5.0, 5.0, 2, 0,		 0.0f,  0.0f,  0.0f,	// 1
		 5.0,  5.0, 5.0, 2, 2,		 0.0f,  0.0f,  0.0f,	// 2
		-5.0,  5.0, 5.0, 0, 2,		 0.0f,  0.0f,  0.0f,	// 3

		// right
		5.0,  5.0,  5.0, 0, 0,		 0.0f,  0.0f,  0.0f,	// 4
		5.0,  5.0, -5.0, 2, 0,		 0.0f,  0.0f,  0.0f,	// 5
		5.0, -5.0, -5.0, 2, 2,		 0.0f,  0.0f,  0.0f,	// 6
		5.0, -5.0,  5.0, 0, 2,		 0.0f,  0.0f,  0.0f,	// 7

		// back
		-5.0,  -5.0, -5.0, 0, 0,	 0.0f,  0.0f,  0.0f,	// 8 
		 5.0,  -5.0, -5.0, 2, 0,	 0.0f,  0.0f,  0.0f,	// 9
		 5.0,   5.0, -5.0, 2, 2,	 0.0f,  0.0f,  0.0f,	// 10
		-5.0,   5.0, -5.0, 0, 2,	 0.0f,  0.0f,  0.0f,	// 11

		// left
		-5.0, -5.0, -5.0, 0, 0,		0.0f,  0.0f,  0.0f,	// 12
		-5.0, -5.0,  5.0, 2, 0,		0.0f,  0.0f,  0.0f,	// 13
		-5.0,  5.0,  5.0, 2, 2,		0.0f,  0.0f,  0.0f,	// 14
		-5.0,  5.0, -5.0, 0, 2,		0.0f,  0.0f,  0.0f,	// 15

		// upper
		 5.0, 5.0,  5.0, 0, 0,		 0.0f,  1.0f,  0.0f,	// 16
		-5.0, 5.0,  5.0, 2, 0,		 0.0f,  1.0f,  0.0f,	// 17
		-5.0, 5.0, -5.0, 2, 2,		 0.0f,  1.0f,  0.0f,	// 18
		 5.0, 5.0, -5.0, 0, 2,		 0.0f,  1.0f,  0.0f,	// 19
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18  // upper
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("lantaiSemen.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords, normal
		// bottom
		-50.0, -0.5, -50.0,  0,  0,  0.0f,  -1.0f,  0.0f,
		 50.0, -0.5, -50.0, 50,  0,	 0.0f,  -1.0f,  0.0f,
		 50.0, -0.5,  50.0, 50, 50,	 0.0f,  -1.0f,  0.0f,
		-50.0, -0.5,  50.0,  0, 50,	 0.0f,  -1.0f,  0.0f,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}
void Demo::BuildLemari() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureLemari);
	glBindTexture(GL_TEXTURE_2D, textureLemari);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("lemari_sisi.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords

		// right
		4.5,  3.0,  -4.5, 0, 0,		1.0f,  0.0f,  0.0f, // 4
		4.5,  3.0,  -3.5, 1, 0,		1.0f,  0.0f,  0.0f, // 5
		4.5, -0.5,  -3.5, 1, 1,		1.0f,  0.0f,  0.0f, // 6
		4.5, -0.5,  -4.5, 0, 1,		1.0f,  0.0f,  0.0f, // 7

		// back
		3.5, -0.5, -4.5, 0, 0,		0.0f,  0.0f,  -1.0f,// 8 
		4.5, -0.5, -4.5, 1, 0,		0.0f,  0.0f,  -1.0f,// 9
		4.5,  3.0, -4.5, 1, 1,		0.0f,  0.0f,  -1.0f,// 10
		3.5,  3.0, -4.5, 0, 1,		0.0f,  0.0f,  -1.0f,// 11

		// left
		3.5, -0.5, -3.5, 0, 0,		-1.0f,  0.0f,  0.0f,// 12
		3.5, -0.5,  -4.5, 1, 0,		-1.0f,  0.0f,  0.0f,// 13
		3.5,  3.0,  -4.5, 1, 1,		-1.0f,  0.0f,  0.0f,// 14
		3.5,  3.0, -3.5, 0, 1,		-1.0f,  0.0f,  0.0f,// 15

		// upper
		4.5, 3.0,  -4.5, 0, 0,		0.0f,  1.0f,  0.0f, // 16
		3.5, 3.0,  -4.5, 1, 0,		0.0f,  1.0f,  0.0f, // 17
		3.5, 3.0, -3.5, 1, 1,		0.0f,  1.0f,  0.0f, // 18
		4.5, 3.0, -3.5, 0, 1,		0.0f,  1.0f,  0.0f, // 19

		// bottom
		3.5, -0.5, -3.5, 0, 0,		0.0f,  -1.0f,  0.0f,// 20
		4.5, -0.5, -3.5, 1, 0,		0.0f,  -1.0f,  0.0f,// 21
		4.5, -0.5,  -4.5, 1, 1,		0.0f,  -1.0f,  0.0f,// 22
		3.5, -0.5,  -4.5, 0, 1,		0.0f,  -1.0f,  0.0f,// 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // right
		4,  5,  6, 4,  6, 7,  // back
		8, 10, 9, 8, 11, 10,  // left
		12, 14, 13, 12, 15, 14,  // upper
		16, 18, 17, 16, 19, 18   // bottom
	};

	glGenVertexArrays(1, &VAOLemari);
	glGenBuffers(1, &VBOLemari);
	glGenBuffers(1, &EBOLemari);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOLemari);

	glBindBuffer(GL_ARRAY_BUFFER, VBOLemari);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOLemari);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::BuildFLemari() {
	// load image into texture memory
		// ------------------------------
		// Load and create a texture 
	glGenTextures(1, &textureFLemari);
	glBindTexture(GL_TEXTURE_2D, textureFLemari);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int widthF, heightF;
	unsigned char* imageF = SOIL_load_image("lemari_depan.png", &widthF, &heightF, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthF, heightF, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageF);
	SOIL_free_image_data(imageF);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float verticesF[] = {
		// format position, tex coords
		// front
		3.5, -0.5, -3.5, 0, 0,		0.0f,  0.0f,  1.0f,// 0
		4.5, -0.5, -3.5, 1, 0,		0.0f,  0.0f,  1.0f,// 1
		4.5,  3.0, -3.5, 1, 1,		0.0f,  0.0f,  1.0f,// 2
		3.5,  3.0, -3.5, 0, 1,		0.0f,  0.0f,  1.0f,// 3

	};

	unsigned int indicesF[] = {
		0,  1,  2,  0,  2,  3,   // front
	};

	glGenVertexArrays(1, &VAOFLemari);
	glGenBuffers(1, &VBOFLemari);
	glGenBuffers(1, &EBOFLemari);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOFLemari);

	glBindBuffer(GL_ARRAY_BUFFER, VBOFLemari);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesF), verticesF, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOFLemari);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesF), indicesF, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

void Demo::DrawMesinCuci() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mcTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mcTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(mcVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawColoredCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawColoredCube2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawColoredCube3()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawTembok1()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawLemari() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureLemari);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureLemari);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(VAOLemari); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawFLemari() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureFLemari);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureFLemari);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(VAOFLemari); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 2.5f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.002f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}
void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}
void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Gudangin", 800, 600, false, false);
}