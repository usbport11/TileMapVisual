#include "../../stdafx.h"
#include "StaticBuffer.h"

MStaticBuffer::MStaticBuffer()
{
	VertexBufferId = -1;
	ColorBufferId = -1;
	BufferSize = 0;
	PrimitiveType = GL_POINTS;
}

MStaticBuffer::~MStaticBuffer()
{
	VertexBufferId = -1;
	ColorBufferId = -1;
	BufferSize = 0;
	PrimitiveType = GL_POINTS;
}

bool MStaticBuffer::Initialize()
{
	GLenum error;
	glGenBuffers(1, &VertexBufferId);
	error = glGetError();
	if(error != GL_NO_ERROR)
	{
		LogFile<<"StaticBuffer: "<<(char*)gluErrorString(error)<<" "<<error<<endl;
		return false;
	}
	
	glGenBuffers(1, &ColorBufferId);
	error = glGetError();
	if(error != GL_NO_ERROR)
	{
		LogFile<<"StaticBuffer: "<<(char*)gluErrorString(error)<<" "<<error<<endl;
		return false;
	}
	
	return true;
}

void MStaticBuffer::SetPrimitiveType(GLenum inType) {
	PrimitiveType = inType;
}

void MStaticBuffer::AddVertex(glm::vec2 Vertex, glm::vec3 Color) {
	Vertexes.push_back(Vertex);
	Colors.push_back(Color);
}

bool MStaticBuffer::Dispose() {
	if(VertexBufferId <= 0 || ColorBufferId <= 0) {
		LogFile<<"Static Buffer: Not initialized"<<endl;
		return false;
	}
	
	//clear and fill buffers
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * Vertexes.size(), &Vertexes[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Colors.size(), &Colors[0], GL_STATIC_DRAW);
	
	BufferSize = Vertexes.size();
	
	return true;
}

void MStaticBuffer::Draw() {
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
	glVertexAttribPointer(SHR_LAYOUT_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glVertexAttribPointer(SHR_LAYOUT_COLOR, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(PrimitiveType, 0, BufferSize);
}

void MStaticBuffer::Begin() {
	glEnableVertexAttribArray(SHR_LAYOUT_VERTEX);
	glEnableVertexAttribArray(SHR_LAYOUT_COLOR);
}

void MStaticBuffer::End() {
	glDisableVertexAttribArray(SHR_LAYOUT_VERTEX);
	glDisableVertexAttribArray(SHR_LAYOUT_COLOR);
}

void MStaticBuffer::Clear() {
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	Vertexes.clear();
	Colors.clear();
}

void MStaticBuffer::Close() {
	Clear();
	glDeleteBuffers(1, &VertexBufferId);
	glDeleteBuffers(1, &ColorBufferId);
}

int MStaticBuffer::GetBufferSize() {
	return BufferSize;
}
