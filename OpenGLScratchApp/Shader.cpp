#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) 
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFilesWithGeo(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometeryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometeryString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	CompileShaderWithGeo(vertexCode, geometryCode, fragmentCode);

}

void Shader::CreateFromFilesWithTess(const char* vertexLocation, const char* tessControlLocation, const char* tessEvaluateLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string tessControlString = ReadFile(tessControlLocation);
	std::string tessEvaluateString = ReadFile(tessEvaluateLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* tessControlCode = tessControlString.c_str();
	const char* tessEvaluateCode = tessEvaluateString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	CompileShaderWithTess(vertexCode, tessControlCode, tessEvaluateCode, fragmentCode);

}


std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}


void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);


	CompileProgram();
}

void Shader::CompileShaderWithGeo(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);


	CompileProgram();
}

void Shader::CompileShaderWithTess(const char* vertexCode, const char* tessControlCode, const char* tessEvaluateCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, tessControlCode, GL_TESS_CONTROL_SHADER); 
	AddShader(shaderID, tessEvaluateCode, GL_TESS_EVALUATION_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);


	CompileProgram();
}

void Shader::CompileProgram()
{

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	//To do: Clarify different shader to get the different uniform location
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformViewPosition = glGetUniformLocation(shaderID, "viewPosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	uniformPtoWTransform = glGetUniformLocation(shaderID, "PToWTransform");

	uniformTessParam = glGetUniformLocation(shaderID, "uniformTessParam");
	uniformTessHeight = glGetUniformLocation(shaderID, "uniformTessHeight");

	uniformDebugFlag = glGetUniformLocation(shaderID, "debugFlag");


	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);

	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directonalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformOmniLightFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (size_t i = 0; i < 6; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		uniformOmniLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}


	for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locBuff);
	}

}

GLuint Shader::GetShaderID()
{	
	if (shaderID != NULL)
	{
		return shaderID;
	}
	else
	{
		return 0;
	}
	
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionLight.uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation()
{
	return uniformDirectionLight.uniformColor;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
}

GLuint Shader::GetViewPosLocation()
{
	return uniformViewPosition;
}

GLuint Shader::GetOmniLightPosLocation()
{
	return uniformOmniLightPos;
}

GLuint Shader::GetOmniLightFarPlaneLocation()
{
	return uniformOmniLightFarPlane;
}

GLuint Shader::GetTessParamLocation()
{
	return uniformTessParam;
}

GLuint Shader::GetTessHeightLocation()
{
	return uniformTessHeight;
}

GLuint Shader::GetDebugFlagLocation()
{
	return uniformDebugFlag;
}



void Shader::setDirectionalLight(DirectionalLight * dLight)
{
	dLight->UseLight(uniformDirectionLight.uniformAmbientIntensity, uniformDirectionLight.uniformColor, 
					uniformDirectionLight.uniformDiffuseIntensity, uniformDirectionLight.uniformDirection);
}


void Shader::setPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) 
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, uniformPointLight[i].uniformDiffuseIntensity,
			uniformPointLight[i].uniformPosition, uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);

		pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLight[i].GetFarPlane());
	}
}

void Shader::setSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, uniformSpotLight[i].uniformDiffuseIntensity,
			uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection, uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear,
			uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformEdge);

		sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, sLight[i].GetFarPlane());
	}

}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::SetPtoWTransform(glm::mat4* p2wTransform)
{
	glUniformMatrix4fv(uniformPtoWTransform, 1, GL_FALSE, glm::value_ptr(*p2wTransform));
}

void Shader::SetViewPostion(glm::vec3* viewPos)
{
	glUniform3fv(uniformViewPosition, 1, glm::value_ptr(*viewPos));
}


void Shader::SetOmniLightMatrices(std::vector<glm::mat4>lightMatrices)
{
	for (size_t i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(uniformOmniLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::SetFloat(const char* propertyName, GLfloat value)
{
	GLint uniformFloat = glGetUniformLocation(shaderID, propertyName);
	if (uniformFloat == -1)
	{
		printf(shaderID + "  doesn't contain property %s", propertyName);
	}
	else
	{
		glUniform1f(uniformFloat, value);
	}
	
}

void Shader::SetTexture(const char* textureName, GLuint value)
{
	GLint uniformTexture = glGetUniformLocation(shaderID, textureName);
	if (uniformTexture == -1)
	{
		printf(shaderID + "  doesn't contain property %s", textureName);
	}
	else
	{
		glUniform1i(uniformTexture, value);
	}
}

void Shader::bindUniformBlockToBindingPoint(const std::string& uniformBlockName, const GLuint bindingPoint)
{
	const auto blockIndex = glGetUniformBlockIndex(shaderID, uniformBlockName.c_str());
	if (blockIndex != GL_INVALID_INDEX) {
		glUniformBlockBinding(shaderID, blockIndex, bindingPoint);
	}
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum saderType)
{
	GLuint theShader = glCreateShader(saderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };


	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", saderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);


}

Shader::~Shader()
{
	ClearShader();
}