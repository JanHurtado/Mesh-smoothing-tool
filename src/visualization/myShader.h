#ifndef MYSHADER_H
#define MYSHADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <GL/glew.h>

using namespace std;

class myShader
{
public:
	myShader();
	myShader(GLenum _shaderType);
	myShader(GLenum _shaderType, const string & _code);
	virtual ~myShader();
	bool readShaderCode(const char * _fileName);
	bool readShaderCode(const string & _fileName);
	void createShader();
	void compileShader();
	
	inline string getShaderCode(){ return m_shaderCode; }
	inline GLuint getShaderID(){ return m_shaderID; }
	inline GLenum getShaderType(){ return m_shaderType; }
	inline void setShaderCode(string _m_shaderCode){ m_shaderCode = _m_shaderCode; }
	inline void setShaderID(GLuint _m_shaderID){ m_shaderID = _m_shaderID; }
	inline void setShaderType(GLenum _m_shaderType){ m_shaderType = _m_shaderType; }

private:
	string m_shaderCode;
	GLuint m_shaderID;
	GLenum m_shaderType;
};

#endif // IOMESH_H