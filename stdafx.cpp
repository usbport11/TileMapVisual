#include "stdafx.h"

ofstream LogFile("log.txt");

bool IsPowerOfTwo(unsigned long x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

bool CheckOpenglSupport() {
	//get opengl data (here was CRASH on suspisious notebook)
	string OpenGLVersion = (char*)glGetString(GL_VERSION);
	string OpenGLVendor = (char*)glGetString(GL_VENDOR);
	string OpenGLRenderer = (char*)glGetString(GL_RENDERER);
	string ShadersVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	LogFile<<"Window: OpenGL version: "<<OpenGLVersion<<endl;
	LogFile<<"Window: OpenGL vendor: "<<OpenGLVendor<<endl;
	LogFile<<"Window: OpenGL renderer: "<<OpenGLRenderer<<endl;
	LogFile<<"Window: Shaders version: "<<ShadersVersion<<endl;
	
	float VersionOGL, VersionSHD;
	sscanf(OpenGLVersion.c_str(), "%f", &VersionOGL);
	if(VersionOGL < 3.0f) {
		LogFile<<"Window: Old version of OpenGL. Sorry"<<endl;
		return false;
	}
	sscanf(ShadersVersion.c_str(), "%f", &VersionSHD);
	if(VersionSHD < 3.3f) {
		LogFile<<"Window: Old version of shaders. Sorry"<<endl;
		return false;
	}
	
	return true;
}
