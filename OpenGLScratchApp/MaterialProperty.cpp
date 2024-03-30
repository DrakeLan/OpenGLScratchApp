#include "MaterialProperty.h"

MaterialProperty::MaterialProperty()
{
	propName = "";
	propType = 0;
	propLocation = 0.0;
	texturePtr = nullptr;
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

