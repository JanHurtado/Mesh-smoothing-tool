#include "myRenderer.h"

///////////////////////COMMON///////////////////////// (begin)

bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar * buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

///////////////////////COMMON///////////////////////// (end)


myRenderer::myRenderer()
{
	//currentDrawFlag = e_draw_faces;
	light = glm::vec4(0.1f,0.1f,0.1f,1.0f);
	lightPosition = glm::vec3(800.f, 800.f, 800.f);
	sceneCentralPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	m_width = 1000;
	m_height = 800;
	m_fov = 45.0f;
	m_near = 1.0f;
	m_far = 1000.0f;
	modelToWorldMatrix = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
}

myRenderer::~myRenderer()
{
	glDeleteBuffers(1, &m_vertexBufferID);
	glDeleteBuffers(1, &m_elementBufferID);
	glUseProgram(0);
	glDeleteProgram(m_programID);
}

void myRenderer::addShape(ShapeData * _sd)
{
	m_shapes.push_back(_sd);
	m_vertexOffsets.push_back(0);
	m_elementOffsets.push_back(0);
	m_vertexArrayObjectIDs.push_back(0);
	m_draw_modes.push_back(e_draw_faces);
}

void myRenderer::addShape(ShapeData * _sd , myDrawFlags _draw_mode)
{
	m_shapes.push_back(_sd);
	m_vertexOffsets.push_back(0);
	m_elementOffsets.push_back(0);
	m_vertexArrayObjectIDs.push_back(0);
	m_draw_modes.push_back(_draw_mode);
}

void myRenderer::removeShape(size_t index)
{
	glDeleteVertexArrays(1, &m_vertexArrayObjectIDs[index]);
	m_shapes.erase(m_shapes.begin() + index);
	m_vertexOffsets.erase(m_vertexOffsets.begin() + index);
	m_elementOffsets.erase(m_elementOffsets.begin() + index);
	m_vertexArrayObjectIDs.erase(m_vertexArrayObjectIDs.begin() + index);
	m_draw_modes.erase(m_draw_modes.begin() + index);
}

void myRenderer::addShader(GLenum _shaderType, const string & _fileName)
{
	myShader * t_ptr_shader = new myShader(_shaderType);
	t_ptr_shader->readShaderCode(_fileName);
	m_shaders.push_back(t_ptr_shader);
}

void myRenderer::addShader(myShader * _shader)
{
	m_shaders.push_back(_shader);
}

void myRenderer::setShapeDrawMode(size_t _shape_index, myDrawFlags _mode)
{
	m_draw_modes[_shape_index] = _mode;
}

void myRenderer::clearShapes()
{
	for (size_t i = 0; i < m_vertexArrayObjectIDs.size(); i++)
	{
		glDeleteVertexArrays(1, &m_vertexArrayObjectIDs[i]);
	}
	m_shapes.clear();
	m_vertexOffsets.clear();
	m_elementOffsets.clear();
	m_vertexArrayObjectIDs.clear();
	m_draw_modes.clear();
}

void myRenderer::clearAndDeleteShapes()
{
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		glDeleteVertexArrays(1, &m_vertexArrayObjectIDs[i]);
		delete m_shapes[i];
	}
	m_shapes.clear();
	m_vertexOffsets.clear();
	m_elementOffsets.clear();
	m_vertexArrayObjectIDs.clear();
	m_draw_modes.clear();
}

void myRenderer::clearShaders()
{
	m_shaders.clear();
}

void myRenderer::clearAndDeleteShaders()
{
	for (size_t i = 0; i < m_shaders.size(); i++)
	{
		glDetachShader(m_programID,m_shaders[i]->getShaderID());
		delete m_shaders[i];
	}
	m_shaders.clear();
}

void myRenderer::createProgram()
{
	m_programID = glCreateProgram();
}

bool myRenderer::installShaders()
{
	for (size_t i = 0; i < m_shaders.size(); i++)
	{
		m_shaders[i]->createShader();
		m_shaders[i]->compileShader();
		if (!checkShaderStatus(m_shaders[i]->getShaderID())) return false;
		glAttachShader(m_programID, m_shaders[i]->getShaderID());
	}
	glLinkProgram(m_programID);
	if (!checkProgramStatus(m_programID)) return false;
	glUseProgram(m_programID);
	return true;
}

void myRenderer::sendDataSingleBuffer()
{
	const uint NUM_FLOATS_PER_VERTICE = 9;
	const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

	GLsizeiptr totalVertexBufferSize = 0;
	GLsizeiptr totalElementBufferSize = 0;
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		totalVertexBufferSize += m_shapes[i]->vertexBufferSize();
		totalElementBufferSize += m_shapes[i]->indexBufferSize();
	}

	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, totalVertexBufferSize, 0, GL_DYNAMIC_DRAW);
	GLsizeiptr currentVertexOffset = 0;
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		m_vertexOffsets[i] = currentVertexOffset;
		glBufferSubData(GL_ARRAY_BUFFER, currentVertexOffset, m_shapes[i]->vertexBufferSize(), m_shapes[i]->vertices);
		currentVertexOffset += m_shapes[i]->vertexBufferSize();
	}

	glGenBuffers(1, &m_elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalElementBufferSize, 0, GL_STATIC_DRAW);
	GLsizeiptr currentElementOffset = 0;
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		m_elementOffsets[i] = currentElementOffset;
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentElementOffset, m_shapes[i]->indexBufferSize(), m_shapes[i]->indices);
		currentElementOffset += m_shapes[i]->indexBufferSize();
	}

	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		//glDeleteVertexArrays(1, &m_vertexArrayObjectIDs[i]);
		glGenVertexArrays(1, &m_vertexArrayObjectIDs[i]);
		
		glBindVertexArray(m_vertexArrayObjectIDs[i]);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(m_vertexOffsets[i]));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(m_vertexOffsets[i] + sizeof(float)* 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(m_vertexOffsets[i] + sizeof(float)* 6));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferID);
	}
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferID);

}

void myRenderer::initialize()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	createProgram();
	sendDataSingleBuffer();
	installShaders();
	initializeInteractor();
}

void myRenderer::initializeInteractor()
{
	computeCentralPoint();
	computeBoundingSphereRadius();
	camera.setPosition(sceneCentralPoint+glm::vec3(0.0f,0.0f,2.0f*boundingSphereRadius));
	camera.setMovementSpeed(boundingSphereRadius/50.0f);
}

void myRenderer::setDefaultValues()
{
	//currentDrawFlag = e_draw_faces;
	sceneCentralPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	m_width = 1000;
	m_height = 800;
	m_fov = 45.0f;
	m_near = 0.01f;
	m_far = 1000.0f;
	modelToWorldMatrix = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	initializeInteractor();
}

void myRenderer::resendDataSingleBuffer()
{
	const uint NUM_FLOATS_PER_VERTICE = 9;
	const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

	GLsizeiptr totalVertexBufferSize = 0;
	GLsizeiptr totalElementBufferSize = 0;
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		totalVertexBufferSize += m_shapes[i]->vertexBufferSize();
		totalElementBufferSize += m_shapes[i]->indexBufferSize();
	}

	//glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, totalVertexBufferSize, 0, GL_DYNAMIC_DRAW);
	GLsizeiptr currentVertexOffset = 0;
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		m_vertexOffsets[i] = currentVertexOffset;
		glBufferSubData(GL_ARRAY_BUFFER, currentVertexOffset, m_shapes[i]->vertexBufferSize(), m_shapes[i]->vertices);
		currentVertexOffset += m_shapes[i]->vertexBufferSize();
	}

	//glGenBuffers(1, &m_elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalElementBufferSize, 0, GL_STATIC_DRAW);
	GLsizeiptr currentElementOffset = 0;
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		m_elementOffsets[i] = currentElementOffset;
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentElementOffset, m_shapes[i]->indexBufferSize(), m_shapes[i]->indices);
		currentElementOffset += m_shapes[i]->indexBufferSize();
	}
	//cout << "vertex buffer : "<< m_vertexBufferID << endl;
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		glGenVertexArrays(1, &m_vertexArrayObjectIDs[i]);
		
		glBindVertexArray(m_vertexArrayObjectIDs[i]);

		//cout << m_vertexArrayObjectIDs[i] << endl;
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(m_vertexOffsets[i]));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(m_vertexOffsets[i] + sizeof(float)* 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(m_vertexOffsets[i] + sizeof(float)* 6));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferID);
	}
}

void myRenderer::draw()
{

	glViewport(0, 0, m_width, m_height);
	//glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glm::mat4 modelToProjectionMatrix;
	glm::mat4 viewToProjectionMatrix = glm::perspective(m_fov, ((float)m_width) / ((float)(m_height)), m_near, m_far);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	// 0: pointSize            1: not defined             2: not defined
	glm::vec3 aditionalProperties = glm::vec3(1.0f,0.0f,0.0f);

	GLuint fullTransformationUniformLocation;
	GLuint modelToWorldMatrixUniformLocation;
	GLuint lightUniformLocation;
	GLuint lightPositionUniformLocation;
	GLuint cameraPositionUniformLocation;
	GLuint aditionalPropertiesUniformLocation;

	fullTransformationUniformLocation = glGetUniformLocation(m_programID, "modelToProjectionMatrix");
	modelToWorldMatrixUniformLocation = glGetUniformLocation(m_programID, "modelToWorldMatrix");
	lightUniformLocation = glGetUniformLocation(m_programID, "lightVector");
	lightPositionUniformLocation = glGetUniformLocation(m_programID, "lightPositionVector");
	cameraPositionUniformLocation = glGetUniformLocation(m_programID, "cameraPositionVector");
	aditionalPropertiesUniformLocation = glGetUniformLocation(m_programID, "aditionalProperties");

	modelToProjectionMatrix = worldToProjectionMatrix * modelToWorldMatrix;

	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &modelToWorldMatrix[0][0]);
	glUniform4fv(lightUniformLocation, 1, &light[0]);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);
	glUniform3fv(cameraPositionUniformLocation, 1, &(camera.getPosition())[0]);
	glUniform3fv(aditionalPropertiesUniformLocation, 1, &aditionalProperties[0]);

	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		glBindVertexArray(m_vertexArrayObjectIDs[i]);
		int currentDrawFlag = m_draw_modes[i];
		switch (currentDrawFlag)
		{
		case e_draw_faces:
			glDrawElements(GL_TRIANGLES, m_shapes[i]->numIndices, GL_UNSIGNED_INT, (void*)(m_elementOffsets[i])); 
			break;
		case e_draw_wireframe: 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, m_shapes[i]->numIndices, GL_UNSIGNED_INT, (void*)(m_elementOffsets[i]));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case e_draw_points:
			glDrawArrays(GL_POINTS, 0, m_shapes[i]->numVertices);
			break;
		case e_draw_selection:
			aditionalProperties[0] = 3.0f;
			glUniform3fv(aditionalPropertiesUniformLocation, 1, &aditionalProperties[0]);
			glDrawArrays(GL_POINTS, 0, m_shapes[i]->numVertices);
			aditionalProperties[0] = 1.0f;
			break;
		}
		glBindVertexArray(0);
	}
}

void myRenderer::updateVertexBuffer(size_t index)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferSubData(GL_ARRAY_BUFFER, m_vertexOffsets[index], m_shapes[index]->vertexBufferSize(), m_shapes[index]->vertices);
}

void myRenderer::computeCentralPoint()
{
	float num = 0.0f;
	glm::vec3 t_sceneCentralPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		for (size_t j = 0; j < m_shapes[i]->numVertices; j++)
		{
			t_sceneCentralPoint += glm::vec3(m_shapes[i]->vertices[j].position.x,m_shapes[i]->vertices[j].position.y,m_shapes[i]->vertices[j].position.z);
			num += 1.0f;
		}
	}
	sceneCentralPoint = t_sceneCentralPoint/num;
}

void myRenderer::computeBoundingSphereRadius()
{
	float max = 0.0f;
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		for (size_t j = 0; j < m_shapes[i]->numVertices; j++)
		{
			glm::vec3 current_point = glm::vec3(m_shapes[i]->vertices[j].position.x, m_shapes[i]->vertices[j].position.y, m_shapes[i]->vertices[j].position.z);
			float dist = glm::distance(current_point,sceneCentralPoint);
			if (dist>max)
				max = dist;
		}
	}
	boundingSphereRadius = max;

}

void myRenderer::rotateObjects(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 20.0f || glm::length(mouseDelta) < 1.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	const float ROTATIONAL_SPEED = 0.1f;

	float valx = mouseDelta.x * ROTATIONAL_SPEED;
	float valy = mouseDelta.y * ROTATIONAL_SPEED;

	glm::mat4 transform = glm::translate(glm::vec3(-sceneCentralPoint.x, -sceneCentralPoint.y, -sceneCentralPoint.z));
	glm::mat4 rotator = glm::rotate(valx, glm::vec3(0.0f,1.0f,0.0f)) * glm::rotate(valy, glm::vec3(1.0f,0.0f,0.0f));
	transform = rotator*transform;
	glm::mat4 temp = translate(glm::vec3(sceneCentralPoint.x, sceneCentralPoint.y, sceneCentralPoint.z));
	transform = temp * transform;
	
	modelToWorldMatrix = transform * modelToWorldMatrix;

	oldMousePosition = newMousePosition;
	return;
}

void myRenderer::translateCamera(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 20.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	float TRANSLATION_SPEED = camera.getMovementSpeed();

	float valx = mouseDelta.x * TRANSLATION_SPEED;
	float valy = mouseDelta.y * TRANSLATION_SPEED;

	camera.setPosition(camera.getPosition()-camera.getUP()*valy+camera.getStrafeDirection()*valx);

	oldMousePosition = newMousePosition;
	return;
}

void myRenderer::zoom(float delta)
{
	if (delta >= 0.0f)
		camera.moveForward();
	else
		camera.moveBackward();
}

glm::vec3 myRenderer::getRayDirection(glm::vec2 & pos)
{
	float x = (2.0f * pos.x) / m_width - 1.0f;
	float y = 1.0f - (2.0f * pos.y) / m_height;
	float z = 1.0f;
	// normalised device space
	glm::vec3 ray_nds = glm::vec3(x, y, z);
	// clip space
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	// eye space
	glm::mat4 proj_mat = glm::perspective(m_fov, ((float)m_width) / ((float)(m_height)), m_near, m_far);
	glm::mat4 view_mat = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getViewDirection(), camera.getUP());

	glm::vec4 ray_eye = inverse(proj_mat) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	// world space
	glm::vec3 ray_wor = glm::vec3(inverse(view_mat) * ray_eye);
	// don't forget to normalise the vector at some point
	ray_wor = glm::normalize(ray_wor);
	return ray_wor;
}