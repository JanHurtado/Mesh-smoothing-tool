#include "myShader.h"

myShader::myShader()
{
	m_shaderType = GL_VERTEX_SHADER;
	m_shaderCode = "";
	m_shaderID = 0;
}

myShader::myShader(GLenum _shaderType)
{
	m_shaderType = _shaderType;
	m_shaderCode = "";
	m_shaderID = 0;
}

myShader::myShader(GLenum _shaderType, const string & _code )
{
	m_shaderType = _shaderType;
	m_shaderCode = _code;
	m_shaderID = 0;
}

myShader::~myShader()
{
	glDeleteShader(m_shaderID);
}

bool myShader::readShaderCode(const char * fileName)
{
	ifstream ifile(fileName);
	if (!ifile.good())
	{
		cout << "Cant read file " << fileName << endl;
		return false;
	}
	m_shaderCode = string(istreambuf_iterator<char>(ifile), istreambuf_iterator<char>());
	return true;
}

bool myShader::readShaderCode(const string & _fileName)
{ 
	return readShaderCode(_fileName.c_str()); 
}

void myShader::createShader()
{
	m_shaderID = glCreateShader(m_shaderType);
}

void myShader::compileShader()
{
	const GLchar* adapter[1];
	adapter[0] = m_shaderCode.c_str();
	glShaderSource(m_shaderID, 1, adapter, 0);
	glCompileShader(m_shaderID);
}