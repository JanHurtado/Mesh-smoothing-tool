#ifndef MYSHADER_H
#define MYSHADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <GL/glew.h>

using namespace std;

/** @addtogroup visualization
  * @brief Shader.
  *
  * @{
  */

/**
 * @brief The myShader class - shader data for OpenGL
 */
class myShader
{
public:
    /**
     * @brief myShader - default constructor
     */
	myShader();
    /**
     * @brief myShader - create empty shader data of a given shader type
     * @param _shaderType shader type
     */
	myShader(GLenum _shaderType);
    /**
     * @brief myShader - create shader data given a shader type and a shader code
     * @param _shaderType
     * @param _code
     */
	myShader(GLenum _shaderType, const string & _code);
    /**
     * @brief ~myShader - destructor
     */
	virtual ~myShader();
    /**
     * @brief readShaderCode - read shader code from an input file (file name as const char *)
     * @param _fileName file name
     * @return true if file can be read
     */
	bool readShaderCode(const char * _fileName);
    /**
     * @brief readShaderCode - read shader code from an input file (file name as string)
     * @param _fileName file name
     * @return true if file can be read
     */
	bool readShaderCode(const string & _fileName);
    /**
     * @brief createShader - create shader in the current OpenGL context
     */
	void createShader();
    /**
     * @brief compileShader - compile shader in the current OpenGL context
     */
	void compileShader();
	
    /////////////////////////////////////////////////
    /// getters
    /////////////////////////////////////////////////
	inline string getShaderCode(){ return m_shaderCode; }
	inline GLuint getShaderID(){ return m_shaderID; }
	inline GLenum getShaderType(){ return m_shaderType; }

    /////////////////////////////////////////////////
    /// setters
    /////////////////////////////////////////////////
	inline void setShaderCode(string _m_shaderCode){ m_shaderCode = _m_shaderCode; }
	inline void setShaderID(GLuint _m_shaderID){ m_shaderID = _m_shaderID; }
	inline void setShaderType(GLenum _m_shaderType){ m_shaderType = _m_shaderType; }

private:
    /**
     * @brief m_shaderCode shader code
     */
	string m_shaderCode;
    /**
     * @brief m_shaderID shader ID in the current OpenGL context
     */
	GLuint m_shaderID;
    /**
     * @brief m_shaderType OpenGL shader type
     */
	GLenum m_shaderType;
};

/** @} */

#endif // IOMESH_H
