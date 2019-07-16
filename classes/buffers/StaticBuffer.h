#ifndef staticbufferH
#define staticbufferH

class MStaticBuffer
{
protected:
	GLuint VertexBufferId;
	GLuint ColorBufferId;
	GLenum PrimitiveType;
	int BufferSize;
	vector<glm::vec2> Vertexes;
	vector<glm::vec3> Colors;
public:
	MStaticBuffer();
	~MStaticBuffer();
	bool Initialize();
	void SetPrimitiveType(GLenum inType);
	void AddVertex(glm::vec2 Vertex, glm::vec3 Color);
	bool Dispose();
	void Draw();
	void Begin();
	void End();
	void Clear();
	void Close();
	int GetBufferSize();
};

#endif

