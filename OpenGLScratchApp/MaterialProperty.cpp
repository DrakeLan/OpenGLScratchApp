#include "MaterialProperty.h"

MaterialProperty::MaterialProperty()
{
	propName = "";
	propType = 0;
	propLocation = 0.0;
	propValue = { 0.0f, 0.0f, 0.0f, 0.0f };
	dirtyFlag = false;
}

MaterialProperty::MaterialProperty(string name, GLint type, GLuint location)
{
	propName = name;
	propType = type;
	propLocation = location;
	propValue = { 0.0f, 0.0f, 0.0f, 0.0f };
	dirtyFlag = false;
}

void MaterialProperty::SetPropValue(float x, float y, float z, float w)
{
	propValue.clear();
	propValue.push_back(x);
	propValue.push_back(y);
	propValue.push_back(z);
	propValue.push_back(w);
	dirtyFlag = true;
}

MaterialProperty::~MaterialProperty()
{
}

