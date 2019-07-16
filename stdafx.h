#ifndef stdafxH
#define stdafxH

//opengl
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gl/gl.h>
#include <gl/glu.h>
//dos
#include <time.h>
#include <stdio.h>
#include <math.h>
//strings
#include <string.h>
//stl
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace std;

//global log file variable - in stdafx.cpp
extern ofstream LogFile;

//structs
struct stVec2Compare {
	bool operator() (const glm::vec2& a, const glm::vec2& b) {
		return (a.x < b.x) || (a.x == b.x && a.y < b.y);
	}
};

//help functions
bool IsPowerOfTwo(unsigned long x);
bool CheckOpenglSupport();

//IMPORTANT!
#define SHR_LAYOUT_VERTEX 0
#define SHR_LAYOUT_UV 1
#define SHR_LAYOUT_COLOR 2

#endif
