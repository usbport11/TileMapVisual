#ifndef shaderH
#define shaderH

struct stStringCompare {
	bool operator() (const char* a, const char* b) {
		return strcmp(a, b) < 0;
	}
};

class MShader
{
private:
	//system
	char* VertexShaderFileName;
	char* FragmentShaderFileName;
	GLuint VertexArrayId;

public:
	//main programm id
	GLuint ProgramId;
	
	//uniforms
	map<const char*, GLuint, stStringCompare> Uniforms;
	bool AddUniform(const char* Name, const char* SearchName);
	
	MShader();
	~MShader();
	GLuint CreateShaderProgram(const char* inVertexShaderFileName, const char* inFragmentShaderFileName);
	bool PrepareShaderValues();
	void Close();
};

#endif
