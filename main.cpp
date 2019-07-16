#include "stdafx.h"
#include "classes/system/Shader.h"
#include "classes/system/Scene.h"
#include "classes/system/FPSController.h"
#include "classes/level/TileMap.h"
#include "classes/buffers/AtlasBuffer.h"
#include "classes/image/TextureLoader.h"

bool Pause;
bool keys[1024] = {0};
int WindowWidth = 800, WindowHeight = 600;
bool EnableVsync = 1;
GLFWwindow* window;
stFPSController FPSController;

int TilesCount[2] = {30, 30};
glm::vec2 Edge(2, 2);
glm::vec2 TileSize(40, 40);

MShader Shader;
MScene Scene;

unsigned int txAtlas_cnt;
stTexture* txAtlas;
MAtlasBuffer AtlasBuffer;
MTileMap TileMap;
MTextureLoader TextureLoader;

bool GenerateLevel() {
	AtlasBuffer.Clear();
	TileMap.Clear();
	
	TilesCount[0] = TilesCount[1] = 9;
	
	TileMap.SetVector(1, 5, 1);
	TileMap.SetVector(1, 6, 1);
	TileMap.SetVector(1, 7, 1);
	TileMap.SetVector(2, 5, 1);
	TileMap.SetVector(2, 6, 1);
	TileMap.SetVector(2, 7, 1);
	
	TileMap.SetVector(3, 5, 1);
	TileMap.SetVector(3, 6, 1);
	TileMap.SetVector(4, 5, 1);
	TileMap.SetVector(4, 6, 1);
	
	TileMap.SetVector(4, 1, 1);
	TileMap.SetVector(4, 2, 1);
	TileMap.SetVector(4, 3, 1);
	TileMap.SetVector(5, 1, 1);
	TileMap.SetVector(5, 2, 1);
	TileMap.SetVector(5, 3, 1);
	TileMap.SetVector(6, 1, 1);
	TileMap.SetVector(6, 2, 1);
	TileMap.SetVector(6, 3, 1);
	
	TileMap.SetVector(5, 4, 1);
	TileMap.SetVector(5, 5, 1);
	TileMap.SetVector(5, 6, 1);
	TileMap.SetVector(5, 7, 1);
	
	//later create floor
	TileMap.CreateWalls();
	
	//fill buffer based on
	unsigned int AtasPos[2];
	for(int i=0; i<TilesCount[0]; i++) {
		for(int j=0; j<TilesCount[1]; j++) {
			if(TileMap.GetValue(i, j) == 0) continue;
			if(TileMap.GetValue(i, j) == 1) {
				AtasPos[0] = 0;
				AtasPos[1] = 1;
			}
			if(TileMap.GetValue(i, j) == 2) {
				AtasPos[0] = 1;
				AtasPos[1] = 0;
			}
			if(TileMap.GetValue(i, j) == 3) {
				AtasPos[0] = 0;
				AtasPos[1] = 0;
			}
			if(!AtlasBuffer.AddData(glm::vec2(i * TileSize.x, j * TileSize.y), glm::vec2((i + 1) * TileSize.x, (j + 1) * TileSize.y), AtasPos[0], AtasPos[1], 0, true)) return false;
		}
	}
	
	//if(!AtlasBuffer.AddData(glm::vec2(0, 0), glm::vec2(64, 64), 0, 1, 0, true)) return false;
	if(!AtlasBuffer.Dispose()) return false;
	return true;
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void mousepos_callback(GLFWwindow* window, double x, double y) {
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}
}

bool InitApp() {
	LogFile<<"Starting application"<<endl;    
    glfwSetErrorCallback(error_callback);
    
    if(!glfwInit()) return false;
    window = glfwCreateWindow(WindowWidth, WindowHeight, "TestApp", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return false;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mousepos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwMakeContextCurrent(window);
    if(glfwExtensionSupported("WGL_EXT_swap_control")) {
    	LogFile<<"Window: V-Sync supported. V-Sync: "<<EnableVsync<<endl;
		glfwSwapInterval(EnableVsync);//0 - disable, 1 - enable
	}
	else LogFile<<"Window: V-Sync not supported"<<endl;
    LogFile<<"Window created: width: "<<WindowWidth<<" height: "<<WindowHeight<<endl;

	//glew
	GLenum Error = glewInit();
	if(GLEW_OK != Error) {
		LogFile<<"Window: GLEW Loader error: "<<glewGetErrorString(Error)<<endl;
		return false;
	}
	LogFile<<"GLEW initialized"<<endl;
	
	if(!CheckOpenglSupport()) return false;

	//shaders
	if(!Shader.CreateShaderProgram("shaders/main.vertexshader.glsl", "shaders/main.fragmentshader.glsl")) return false;
	if(!Shader.AddUniform("MVP", "MVP")) return false;
	if(!Shader.AddUniform("TextureId", "myTextureSampler")) return false;
	LogFile<<"Shaders loaded"<<endl;

	//scene
	if(!Scene.Initialize(&WindowWidth, &WindowHeight)) return false;
	LogFile<<"Scene initialized"<<endl;

	//randomize
    srand(time(NULL));
    LogFile<<"Randomized"<<endl;
    
    //other initializations
    //load textures
    txAtlas = TextureLoader.LoadTexture("textures/tex04.png", 1, 1, 0, txAtlas_cnt, GL_NEAREST, GL_REPEAT);
    if(!txAtlas) return false;
    //init buffers
	if(!AtlasBuffer.Initialize(txAtlas, 32, 32, 2, 2)) return false;
    //init map
    TileMap = MTileMap(TilesCount[0], TilesCount[1]);
    //generate level
	if(!GenerateLevel()) return false;
	
	//turn off pause
	Pause = false;
    
    return true;
}

void RenderStep() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(Shader.ProgramId);
	glUniformMatrix4fv(Shader.Uniforms["MVP"], 1, GL_FALSE, Scene.GetDynamicMVP());
	glUniform1i(Shader.Uniforms["TextureId"], 0);
	
	//draw functions
	AtlasBuffer.Begin();
	AtlasBuffer.Draw();
	AtlasBuffer.End();
}

void ClearApp() {
	//clear funstions
	AtlasBuffer.Close();
	TileMap.Close();
	TextureLoader.DeleteTexture(txAtlas, txAtlas_cnt);
	TextureLoader.Close();
	
	memset(keys, 0, 1024);
	Shader.Close();
	LogFile<<"Application: closed"<<endl;
}

int main(int argc, char** argv) {
	LogFile<<"Application: started"<<endl;
	if(!InitApp()) {
		ClearApp();
		glfwTerminate();
		LogFile.close();
		return 0;
	}
	FPSController.Initialize(glfwGetTime());
	while(!glfwWindowShouldClose(window)) {
		FPSController.FrameStep(glfwGetTime());
    	FPSController.FrameCheck();
		RenderStep();
        glfwSwapBuffers(window);
        glfwPollEvents();
	}
	ClearApp();
    glfwTerminate();
    LogFile.close();
}
