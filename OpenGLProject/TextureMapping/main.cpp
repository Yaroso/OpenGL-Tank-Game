// Includes
#include <GL/glew.h>
#include <stdlib.h>
#include <GL/glut.h>	//OpenGL Related Functions
#include <Shader.h>		//Functions for compiling shaders
#include <Vector.h>		//Functions for Vectors
#include <iostream>		//Writing to the Console using std::cout 
#include <math.h>       // Includes Math functions
#include <Matrix.h>
#include <Mesh.h>
#include <SphericalCameraManipulator.h>
#include <Texture.h>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <iomanip>

//Function Prototypes
bool initGL(int argc, char** argv);		        	//Function to init OpenGL
void initShader();					                //Function to init Shader
void display(void);					                //OpenGL Display Function
void keyboard(unsigned char key, int x, int y);		//Keyboard Function
void mouse(int button, int state, int x, int y);	//Mouse Function
void motion(int x, int y);				            //Mouse Motion Function
void initTexture(std::string filename, GLuint & textureID); 
void readMazeFromFile(std::string filepath);
void drawTank(Matrix4x4); //all the obj from main
void drawTurret(Matrix4x4);
void drawMaze(Matrix4x4); //from Maze[][]
void drawWheels(Matrix4x4); //all the obj from main
void drawShell(Matrix4x4);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void specialKeyboardUp(int key, int x, int y);
void moveTank();
void moveTankWS(unsigned char key);
void moveTurretKeys(int specialKey);
void moveTurret();
int checkPosition();
void fallingTank();
void resetTank();
void render2dText(std::string text, float r, float g, float b,
	float x, float y);
void moveShell();
void dispText();
double getRemainingTime();
void createMenu();
void menuHandler(int);


//Global Variables
GLuint shaderProgramID;			                    // Shader Program ID
GLuint vertexPositionAttribute;		                // Vertex Position Attribute Location
GLuint vertexTexcoordAttribute; // Vertex Texcoord Attribute Location
GLuint TextureMapUniformLocation; // Texture Map Location
GLuint textureBox; // OpenGL Texture Box
GLuint textureCoin; // OpenGL Texture Coin
GLuint textureTank; // OpenGL Texture Box
GLuint textureShell; // OpenGL Texture Box
GLuint vertexNormalAttribute;
GLuint texture;

//Material Properties
GLuint LightPositionUniformLocation;                // Light Position Uniform   
GLuint AmbientUniformLocation;
GLuint SpecularUniformLocation;
GLuint SpecularPowerUniformLocation;

//Lighting Phong
Vector3f lightPosition = Vector3f(20.0, 20.0, 20.0);   
Vector3f ambient = Vector3f(0.07, 0.07, 0.07);         
Vector3f specular = Vector3f(1.0, 1.0, 1.0);        
float specularPower = 10.0;                         
            	
//Viewing
SphericalCameraManipulator cameraManip;
Matrix4x4 ModelViewMatrix;		                    // ModelView Matrix
GLuint MVMatrixUniformLocation;		                // ModelView Matrix Uniform
Matrix4x4 ProjectionMatrix;		                    // Projection Matrix
GLuint ProjectionUniformLocation;	                // Projection Matrix Uniform Location

//Mesh
Mesh meshBox;				                            // Mesh Box
Mesh meshCoin;										// Mesh Coin
Mesh meshBack_wheel, meshFront_wheel, meshChassis, meshTurret; // Mesh Tank
Mesh meshShell;

//size of the maze
const int MazeSize = 10;

std::string Maze_path;

// Initialize the maze as a global variable (it will be overwritten by the file)
int Maze[MazeSize][MazeSize] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
//Copy of the Maze (for reset)
int Maze_initial[MazeSize][MazeSize] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int gameStatus = 0; //0 - for game running, 1 - you win, 2 - you lose, 3 - try again
bool restartKey = false; //the r key by default

double elapsedTime = 0.0;
double countdownReset = 0.0;

double levelDuration = 50000.0; // 40 seconds

// Define a variable to store the starting time of the level
double startTime = 0.0;

//for distance within the box or the coin when it should register
float offset = 0.25f; //prev = 0.35f

//Tracking all the keyboard keys
bool keyWPressed = false;
bool keyAPressed = false;
bool keySPressed = false;
bool keyDPressed = false;

// Variables for coins
int coinsInit = 0;
float coinsAngle = 0.0f;
int coinsCollected = 0;
int coinsLoop = 0;

//Tank position glob variables. TO DO put in a separate class
float tankX = 0.0f;
float tankY = 0.85f;
float tankZ = 0.0f;

//Shell position global variables 
float shellX = 0.0f;
float shellY = 0.85f;
float shellZ = 0.0f;

//variables for Tank Movement & Rotation
float tankSpeedForward = 0.1f;
int tankRotateState = 0;
float rotateAngle = 0.0f;
int moveAndRotate = 0;

//Turret rotation variables
float turretRotationSpeed = 0.05f; // Adjust this value for desired rotation speed
float turretAngle = 0.0f;
int turretRotateState = 0;
float turretAngleVertical = 0.0f;
float maxTurretAngleUp = 0.0f; // Maximum angle the turret can rotate up (in degrees)
float maxTurretAngleDown = -17.0f; // Maximum angle the turret can rotate down (in degrees)

//Wheels variables
float wheelsAngle = 0.0f;
float wheelsSpeed = 1.0f;

//Shell parameters
bool shellShot = false;
int shellDrawInit = 0;
float shellSpeed = 0.1f;
float shellRotateAngle = 0.0f;
bool shellOutOfBound = false;

//Main Program Entry
int main(int argc, char** argv)
{	
	//init OpenGL
	if(!initGL(argc, argv))
		return -1;
    
    //Init OpenGL Shader
    initShader();
	
	//Init Mesh Geometry
    meshBox.loadOBJ("assets/cube.obj");
	meshCoin.loadOBJ("assets/coin.obj");
	meshBack_wheel.loadOBJ("assets/back_wheel.obj");
	meshFront_wheel.loadOBJ("assets/front_wheel.obj");
	meshChassis.loadOBJ("assets/chassis.obj");
	meshTurret.loadOBJ("assets/turret.obj");
	meshShell.loadOBJ("assets/ball.obj");

	Maze_path = "assets/maze1.txt";
	readMazeFromFile(Maze_path);

	createMenu();

    //Glueing camera to the tank
	cameraManip.setPanTiltRadius(0.f, -0.5f * 3.14, 4.f);
	cameraManip.setFocus(meshChassis.getMeshCentroid());
	
	initTexture("assets/Crate.bmp", textureBox);
	initTexture("assets/coin.bmp", textureCoin);
	initTexture("assets/hamvee.bmp", textureTank);
	initTexture("assets/ball.bmp", textureShell);

	//Start main loop
	glutMainLoop();
	
	//Clean-Up
	glDeleteProgram(shaderProgramID);

    return 0;
}

//! Function to Initlise OpenGL
bool initGL(int argc, char** argv)
{
	//Init GLUT
    glutInit(&argc, argv);
    
	//Set Display Mode: single buffered
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	
	//Set Window Size
    glutInitWindowSize(600, 600);
    
    // Window Position
    glutInitWindowPosition(300, 0);

	//Create Window
    glutCreateWindow("Tank Game");
    
    // Init GLEW
	if (glewInit() != GLEW_OK) 
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	
	//Set Display function
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKeyboard);
	glutSpecialUpFunc(specialKeyboardUp);
	glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glEnable(GL_TEXTURE_2D);
	glClearColor(0.67, 0.85, 0.9, 0.0);

	glEnable(GL_DEPTH_TEST);
	
	return true;
}

//Init Shader
void initShader()
{
	//Create shader
	shaderProgramID = Shader::LoadFromFile("assets/shader.vert","assets/shader.frag");

	vertexPositionAttribute = glGetAttribLocation(shaderProgramID, "aVertexPosition");
	vertexNormalAttribute = glGetAttribLocation(shaderProgramID, "aVertexNormal");
	vertexTexcoordAttribute = glGetAttribLocation(shaderProgramID, "aVertexTexcoord");

	MVMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "MVMatrix_uniform");
	ProjectionUniformLocation = glGetUniformLocation(shaderProgramID, "ProjMatrix_uniform");
	LightPositionUniformLocation = glGetUniformLocation(shaderProgramID, "LightPosition_uniform");
	AmbientUniformLocation = glGetUniformLocation(shaderProgramID, "Ambient_uniform");
	SpecularUniformLocation = glGetUniformLocation(shaderProgramID, "Specular_uniform");
	SpecularPowerUniformLocation = glGetUniformLocation(shaderProgramID, "SpecularPower_uniform");
	TextureMapUniformLocation = glGetUniformLocation(shaderProgramID, "TextureMap_uniform");	
}

//Init texture
void initTexture(std::string filename, GLuint& textureID)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Get texture Data
	int width, height;
	char* data;
	Texture::LoadBMP(filename, width, height, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
		data);

	//Cleanup data as copied to GPU
	delete[] data;
}

//Display Loop
void display(void)
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Use shader
	glUseProgram(shaderProgramID);

	//Projection Matrix - Perspective Projection
	ProjectionMatrix.perspective(90, 1.0, 0.1, 1000.0);

	//Set Projection Matrixc
	glUniformMatrix4fv(
		ProjectionUniformLocation,  //Uniform location
		1,							//Number of Uniforms
		false,						//Transpose Matrix
		ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues

	//cameraManip.setFocus(meshChassis.getMeshCentroid());
	cameraManip.setFocus(Vector3f(tankX, 1.15f + tankY, tankZ));

	//Apply Camera Manipluator to Set Model View Matrix on GPU
	ModelViewMatrix.toIdentity();
	Matrix4x4 m = cameraManip.apply(ModelViewMatrix);
	glUniformMatrix4fv(
		MVMatrixUniformLocation,  	//Uniform location
		1,					        //Number of Uniforms
		false,				        //Transpose Matrix
		m.getPtr());	        //Pointer to Matrix Values

	glUniform3f(LightPositionUniformLocation, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform4f(AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
	glUniform4f(SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
	glUniform1f(SpecularPowerUniformLocation, specularPower);

	//Set Colour after program is in use
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(TextureMapUniformLocation, 0);

	drawTank(m);
	drawMaze(m);

	coinsAngle += 0.2f; // Number stands for rotation speed of the coins

	if (checkPosition() == 0) {
		gameStatus = 3;
		fallingTank();
	}
	else if (checkPosition() == 1 && restartKey == false && gameStatus != 2){
		gameStatus = 0;
		moveTank();
		moveTurret();

		if (shellShot == true) {
			drawShell(m);
			moveShell();
		}
	}
	else if (gameStatus == 2) {
		gameStatus = 0;
		resetTank();
	}

	if (coinsCollected == coinsInit) {
		gameStatus = 1;
	}

	if (getRemainingTime() == 0.0) {
		gameStatus = 2;
	}

	//Unuse Shader
	glUseProgram(0);

	dispText();

	//Redraw frame
	glutPostRedisplay();
	glutSwapBuffers();
}

// Keyboard Interaction
void keyboard(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		keyWPressed = true;
		wheelsAngle += wheelsSpeed;
	}
	if (key == 'a' || key == 'A')
	{
		keyAPressed = true;
		tankRotateState = 1;
	}
	if (key == 's' || key == 'S')
	{
		keySPressed = true;
		wheelsAngle -= wheelsSpeed;
	}
	if (key == 'd' || key == 'D')
	{
		keyDPressed = true;
		tankRotateState = 2;
	}

	if (key == ' ') // Space bar
	{
		shellShot = true;
		shellDrawInit = 0;
	}
	if (key == 'r' || key == 'R')
	{
		restartKey = true;
		resetTank();
		countdownReset = glutGet(GLUT_ELAPSED_TIME);
	}
	
	//Tell opengl to redraw frame
	glutPostRedisplay();
}

// Mouse Interaction
void mouse(int button, int state, int x, int y)
{
    cameraManip.handleMouse(button, state,x,y);
    glutPostRedisplay(); 
}

// Mouse Interaction
void motion(int x, int y)
{	
    cameraManip.handleMouseMotion(x,y);
    glutPostRedisplay(); 
}

void readMazeFromFile(std::string filepath){

	std::ifstream filestream(filepath);

	if (!filestream.is_open()){
		std::cerr << "Error: Unable to open maze file " << std::endl;
		return;
	}
	
	std::string line;
	int row = 0;

	while (getline(filestream, line)) {
		int column = 0;
		for (char c : line) {
			if (isdigit(c)) {
				Maze[row][column] = c - '0'; //from char to int
				++column;
				if (column == MazeSize) break;
			}
		}
		++row;
		if (row == MazeSize) break;
	}

	for (int i = 0; i < MazeSize; i++) {
		for (int j = 0; j < MazeSize; j++) {
			Maze_initial[i][j] = Maze[i][j];
		}
	}

	filestream.close();
}

void drawMaze(Matrix4x4 m) {
	Matrix4x4 m_temp = m;

	// Drawing the maze
	for (int i = 0; i < MazeSize; ++i) {
		for (int j = 0; j < MazeSize; ++j) {
			// If the maze cell value is 1, draw a mesh
			if (Maze[i][j] == 1) {

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureBox);
				glUniform1i(TextureMapUniformLocation, 0);

				// Calculate translation for the second mesh
				m.translate(2.0f * j, 0.0f, 2.0f * i); 

				// Apply translation to the ModelViewMatrix
				ModelViewMatrix = ModelViewMatrix * m;

				// Set the model-view matrix uniform for the translated mesh
				glUniformMatrix4fv(
					MVMatrixUniformLocation,
					1,
					false,
					m.getPtr());

				meshBox.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

				m = m_temp;
			}

			if (Maze[i][j] == 2) {
				//Drawing the box
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureBox);
				glUniform1i(TextureMapUniformLocation, 0);

				// Calculate translation for the second mesh
				m.translate(2.0f * j, 0.0f, 2.0f * i);

				// Apply translation to the ModelViewMatrix
				ModelViewMatrix = ModelViewMatrix * m;

				// Set the model-view matrix uniform for the translated mesh
				glUniformMatrix4fv(
					MVMatrixUniformLocation,
					1,
					false,
					m.getPtr());

				meshBox.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

				//Drawing the coin 
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureCoin);
				glUniform1i(TextureMapUniformLocation, 0);

				m.translate(0.0f, 1.6f, 0.0f); // so that it is where needed on the box
				m.rotate(coinsAngle, 0.0f, 1.0f, 0.0f); //Making sure it is above the box
				m.scale(0.4f, 0.4f, 0.4f);

				// Apply translation to the ModelViewMatrix
				ModelViewMatrix = ModelViewMatrix * m;

				// Set the model-view matrix uniform for the translated mesh
				glUniformMatrix4fv(
					MVMatrixUniformLocation,
					1,
					false,
					m.getPtr());

				meshCoin.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

				m = m_temp;
			}
		}
	}

	for (coinsLoop; coinsLoop < 1; coinsLoop++) {
		for (int i = 0; i < MazeSize; ++i) {
			for (int j = 0; j < MazeSize; ++j) {
				if (Maze[i][j] == 2) {
					coinsInit++;
				}
			}
		}
	}

	
}

void drawTank(Matrix4x4 m) {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTank);
	glUniform1i(TextureMapUniformLocation, 0);

	m.translate(tankX, tankY, tankZ); //Making sure it is above the box
	m.scale(0.3f, 0.3f, 0.3f);

	//So that the tank rotates but turret doesn't
	Matrix4x4 m_temp_turret = m;

	m.rotate(rotateAngle, 0.0f, 1.0f, 0.0f);

	// Apply translation to the ModelViewMatrix
	ModelViewMatrix = ModelViewMatrix * m;

	// Set the model-view matrix uniform for the translated mesh
	glUniformMatrix4fv(
		MVMatrixUniformLocation,
		1,
		false,
		m.getPtr());

	meshChassis.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

	drawTurret(m_temp_turret);
	drawWheels(m);
}

void drawTurret(Matrix4x4 m_turret) {

	Matrix4x4 turretMatrix = m_turret;

	float cameraPan = cameraManip.getPan() * (180.0f / 3.14f);

	turretAngle = fabs(cameraPan);

	turretMatrix.rotate(fabs(cameraPan), 0.0f, 1.0f, 0.0f); // Rotate around Y-axis

	ModelViewMatrix = ModelViewMatrix * turretMatrix;

	glUniformMatrix4fv(
		MVMatrixUniformLocation,
		1,
		false,
		turretMatrix.getPtr());
	
	meshTurret.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
}

void drawWheels(Matrix4x4 m_wheels) {

	Matrix4x4 wheelsMatrixFront = m_wheels;
	Matrix4x4 wheelsMatrixBack = m_wheels;

	Matrix4x4 ModelViewMatrixInit = ModelViewMatrix;

	float rotationAxisFront = 2.10f;

	wheelsMatrixFront.translate(0.0f, 1.0f, rotationAxisFront); 

	//rotation around the front wheels
	wheelsMatrixFront.rotate(wheelsAngle, 1.0, 0.0f, 0.0f); 
	wheelsMatrixFront.translate(0.0f, -1.0f, -rotationAxisFront); // translating back

	ModelViewMatrix = ModelViewMatrixInit * wheelsMatrixFront;

	glUniformMatrix4fv(
		MVMatrixUniformLocation,
		1,
		false,
		wheelsMatrixFront.getPtr());

	meshFront_wheel.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

	
	//rotation around the back wheels
	float rotationAxisBack = -1.25f;

	wheelsMatrixBack.translate(0.0f, 1.0f, rotationAxisBack);

	//rotation around the front wheels
	wheelsMatrixBack.rotate(wheelsAngle, 1.0, 0.0f, 0.0f);
	wheelsMatrixBack.translate(0.0f, -1.0f, -rotationAxisBack); // translating back

	ModelViewMatrix = ModelViewMatrixInit * wheelsMatrixBack;

	glUniformMatrix4fv(
		MVMatrixUniformLocation,
		1,
		false,
		wheelsMatrixBack.getPtr());

	meshBack_wheel.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
	
}

void keyboardUp(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		keyWPressed = false;
	}

	if (key == 'a' || key == 'A')
	{
		tankRotateState = 0;
		keyAPressed = false;
	}

	if (key == 's' || key == 'S')
	{
		keySPressed = false;
	}

	if (key == 'd' || key == 'D')
	{
		tankRotateState = 0;
		keyDPressed = false;
	}
	if (key == 'r' || key == 'R')
	{
		restartKey = false;
	}
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		turretRotateState = 1;
		moveTurretKeys(key);

	}
	else if (key == GLUT_KEY_RIGHT) {
		turretRotateState = 2;
		moveTurretKeys(key);
	}
	if (key == GLUT_KEY_UP) {
		turretRotateState = 3;
		moveTurretKeys(key);

	}
	else if (key == GLUT_KEY_DOWN) {
		turretRotateState = 4;
		moveTurretKeys(key);
	}
} 

void specialKeyboardUp(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT || key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
		turretRotateState = 0;
	}
}

void moveTank() {
	//Movememnt of the tank just w and s
	if (keyWPressed == true && tankRotateState == 0) {
		moveTankWS('w');
	}

	if (keySPressed == true && tankRotateState == 0) {
		moveTankWS('s');
	}

	//movement when a is included
	if (keyWPressed == true && tankRotateState == 1) {
		moveTankWS('w');
		rotateAngle += 0.05f;
	}

	if (keySPressed == true && tankRotateState == 1) {
		moveTankWS('s');
		rotateAngle += 0.05f;
	}

	if (keyWPressed == false && tankRotateState == 1) {
		rotateAngle += 0.05f;
	}

	if (keySPressed == false && tankRotateState == 1) {
		rotateAngle += 0.05f;
	}

	//movement when d is included
	if (keyWPressed == true && tankRotateState == 2) {
		moveTankWS('w');
		rotateAngle -= 0.05f;
	}

	if (keySPressed == true && tankRotateState == 2) {
		moveTankWS('s');
		rotateAngle -= 0.05f;
	}

	if (keyWPressed == false && tankRotateState == 2) {
		rotateAngle -= 0.05f;
	}

	if (keySPressed == false && tankRotateState == 2) {
		rotateAngle -= 0.05f;
	}
}

void moveTankWS(unsigned char key) {
	if (key == 'w') {
		float rotateAngleRad = rotateAngle * 3.14 / 180.0;
		float deltaX = tankSpeedForward * sin(rotateAngleRad) * 0.05;
		float deltaZ = tankSpeedForward * cos(rotateAngleRad) * 0.05;

		// Update tank position
		tankX += deltaX;
		tankZ += deltaZ;

		wheelsAngle += wheelsSpeed;
	}
	if (key == 's') {
		float rotateAngleRad = rotateAngle * 3.14 / 180.0;
		float deltaX = tankSpeedForward * sin(rotateAngleRad) * 0.05;;
		float deltaZ = tankSpeedForward * cos(rotateAngleRad) * 0.05;;

		// Update tank position
		tankX -= deltaX;
		tankZ -= deltaZ;

		wheelsAngle -= wheelsSpeed;
	}
}

void moveTurretKeys(int specialKey) {
	if (specialKey == GLUT_KEY_LEFT) {
		turretAngle -= turretRotationSpeed;
		//std::cout << "in moveTurret " << std::endl;
	}
	else if (specialKey == GLUT_KEY_RIGHT) {
		turretAngle += turretRotationSpeed;
	}
	else if (specialKey == GLUT_KEY_UP) {
		turretAngleVertical -= turretRotationSpeed;
	}
	else if (specialKey == GLUT_KEY_DOWN) {
		turretAngleVertical += turretRotationSpeed;
	}
}

void moveTurret() {
	//For rotation of the turret 
	if (turretRotateState == 1) {
		turretAngle += 0.1f;
	}

	if (turretRotateState == 2) {

		turretAngle -= 0.1f;
	}

	turretAngleVertical = fmodf(turretAngleVertical, 360.0f); // Wrap to 0-360 degrees

	if (turretRotateState == 3) { //up button
		if (turretAngleVertical < maxTurretAngleUp) {
			turretAngleVertical += 0.05f;
		}
		else { turretAngleVertical = maxTurretAngleUp; }
	}

	if (turretRotateState == 4) { //down button
		if (turretAngleVertical > maxTurretAngleDown) {
			turretAngleVertical -= 0.05f;
		}
		else { turretAngleVertical = maxTurretAngleDown; }
	}
}

int checkPosition(){
	float local_tankX = tankX/2.0f; //+offset;
	float local_tankZ = tankZ/2.0f; // +offset;

	float local_shellX = shellX / 2.0f; //+offset;
	float local_shellZ = shellZ / 2.0f; // +offset;

	if (tankZ < -2.0f || tankZ > 20.0f || tankX < -2.0f  || tankX > 20.0f ) {
		return 0;
	}

	for (int i = 0; i < MazeSize; ++i) {
		for (int j = 0; j < MazeSize; ++j) {
			// Check if the current position in the maze is where the tank should fall
			if (Maze[i][j] == 0) {
				if ((fabs(local_tankX - j) < offset) && (fabs(local_tankZ - i) < offset)) {
					//std::cout << "inside if j: " << j<< "	inside if i: " << i<< std::endl;
					return 0; // Tank should fall
				}
			}
			if (Maze[i][j] == 2) {
				//a tank collects a coin
				if ((fabs(local_tankX - j) < offset) && (fabs(local_tankZ - i) < offset)) {
					coinsCollected++;
					Maze[i][j] = 1;
					return 1;
				}
				//a shell hits a coin
				if ((fabs(local_shellX - j) < offset) && (fabs(local_shellZ - i) < offset)) {
					coinsCollected++;
					Maze[i][j] = 1;
					return 1;
				}
			}
		}
	}

	return 1; // Tank should not fall
}

void fallingTank(){
	if (tankY > -8.0f) {
		tankY -= 0.005f; // fall speed
	}
}

void resetTank() {
	//brings the tank back to the initial position
	tankX = 0.0f;
	tankY = 0.85f;
	tankZ = 0.0f;
	coinsCollected = 0;
	rotateAngle = 0.0f;
	
	coinsInit = 0;
	coinsLoop = 0;

	for (int i = 0; i < MazeSize; i++) {
		for (int j = 0; j < MazeSize; j++) {
			Maze[i][j] = Maze_initial[i][j];
		}
	}
}

void render2dText(std::string text, float r, float g, float b,
	float x, float y)
{
	glColor3f(r, g, b);
	
	glRasterPos2f(x, y); //top left corner

	// Each character of the text gets rendered
	for (unsigned int i = 0; i < text.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

void drawShell(Matrix4x4 s) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureShell);
	glUniform1i(TextureMapUniformLocation, 0);

	for (shellDrawInit; shellDrawInit < 1; shellDrawInit++) {
		shellOutOfBound = false;

		shellX = tankX;
		shellY = tankY + 0.87f;
		shellZ = tankZ;

		shellRotateAngle = turretAngle;

		//Shell appears where needed (same from drawTurret)
		s.rotate(turretAngle, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
		float cameraPanShell = cameraManip.getPan() * (180.0f / 3.14f);
		s.rotate(fabs(cameraPanShell), 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
	}

	//s.translate(shellX, shellY, shellZ); 
	s.translate(shellX, shellY, shellZ);
	s.scale(0.075f, 0.075f, 0.075f);  

	// Apply translation to the ModelViewMatrix
	ModelViewMatrix = ModelViewMatrix * s;

	// Set the model-view matrix uniform for the translated mesh
	glUniformMatrix4fv(
		MVMatrixUniformLocation,
		1,
		false,
		s.getPtr());

	meshShell.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
}

void moveShell() {
	if (shellShot == true && shellOutOfBound == false) {
		float shellRotateAngleRad = shellRotateAngle * 3.14 / 180.0;
		float deltaShellX = shellSpeed * sin(shellRotateAngleRad) * 0.5;
		float deltaShellZ = shellSpeed * cos(shellRotateAngleRad) * 0.5;

		if (shellZ < -2.0f || shellZ > 20.0f || shellX < -2.0f || shellX > 20.0f) {
			shellShot = false;
			shellOutOfBound = true;
		}

		// Update shell position
		shellX += deltaShellX;
		shellZ += deltaShellZ;
		shellY -= 0.002f;
	}
}

void dispText() {
	render2dText("Coins collected: " + std::to_string(coinsCollected), 1.0f, 1.0f, 1.0f, -0.95, 0.9);

	double remainingTime = getRemainingTime();

	auto seconds = remainingTime / 1000.0; // Milliseconds to seconds

	// time up to 3 decimals
	std::stringstream ss;
	ss << std::fixed << std::setprecision(3) << seconds; // Set precision to 3 decimal places
	render2dText("Time: " + ss.str(), 1.0f, 1.0f, 1.0f, -0.95, 0.8);

	// tank coords
	std::stringstream ssCoordinates;
	ssCoordinates << std::fixed << std::setprecision(3) << "Tank X: " << tankX << ", Tank Y: " << tankY << ", Tank Z: " << tankZ;
	render2dText(ssCoordinates.str(), 1.0f, 1.0f, 1.0f, -0.95, 0.7);

	if (gameStatus == 1) {
		render2dText("You Win!", 1.0f, 1.0f, 1.0f, -0.2, 0.0);
	}
	else if (gameStatus == 2) {
		render2dText("You Lose!", 1.0f, 1.0f, 1.0f, -0.2, 0.0);
	}
	else if (gameStatus == 3) {
		render2dText("Try Again!", 1.0f, 1.0f, 1.0f, -0.2, 0.0);
	}

}

double getRemainingTime() {
	double timeTrack = glutGet(GLUT_ELAPSED_TIME);
	double remainingTime = levelDuration - (timeTrack - countdownReset);
	return remainingTime > 0 ? remainingTime : 0;
}

void createMenu() {
	int menu = glutCreateMenu(menuHandler); // Create the menu

	glutAddMenuEntry("Level 1", 1);
	glutAddMenuEntry("Level 2", 2);
	glutAddMenuEntry("Level 3", 3);

	// middle mouse button for menu
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void menuHandler(int choice) {
	switch (choice) {
	case 1:
		// Load Level 1
		Maze_path = "assets/maze1.txt";
		readMazeFromFile(Maze_path);
		
		countdownReset = glutGet(GLUT_ELAPSED_TIME);
		levelDuration = 50000;
		resetTank();
		break;
	case 2:
		// Load Level 2
		Maze_path = "assets/maze2.txt";
		readMazeFromFile(Maze_path);

		countdownReset = glutGet(GLUT_ELAPSED_TIME);
		levelDuration = 30000;
		resetTank();
		break;
	case 3:
		// Load Level 3
		Maze_path = "assets/maze3.txt";
		readMazeFromFile(Maze_path);

		countdownReset = glutGet(GLUT_ELAPSED_TIME);
		levelDuration = 20000;
		resetTank();
	}
}