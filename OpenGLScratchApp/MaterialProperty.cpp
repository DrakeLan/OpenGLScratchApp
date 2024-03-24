#include "MaterialProperty.h"

MaterialProperty::MaterialProperty()
{
	propName = "";
	propType = 0;
	propLocation = 0.0;
}

MaterialProperty::MaterialProperty(string name, GLint type, GLuint location)
{
	propName = name;
	propType = type;
	propLocation = location;
}

MaterialProperty::~MaterialProperty()
{
}

template<typename... valueType>
void MaterialProperty::SetPropValue(valueType ... value)
{
	propValue.clear();
	propValue.push_back(value);

	/*switch (propType)
	{
	case GL_FLOAT:
		
		propValue.resize(1);
		break;
	case GL_FLOAT_VEC2:
		propValue.push_back(value);
		propValue.resize(2);
		break;
	case GL_FLOAT_VEC3:
		glUniform3f(curLocation, value.x, value.y, value.z);
		break;
	case GL_FLOAT_VEC4:
		glUniform4f(curLocation, value.x, value.y, value.z, value.w);
		break;
	case GL_INT || GL_BOOL:
		glUniform1i(curLocation, value);
		break;
	default:
		printf("The property is not correct type!");
		break;
	}*/
}
