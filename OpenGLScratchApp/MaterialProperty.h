#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <GL\glew.h>
#include <type_traits>
#include "Textrue.h"

using namespace std;

class MaterialProperty
{
	public:
		MaterialProperty();
		MaterialProperty(string name, GLint type, GLuint location);

		string propName;
		GLint propType;
		vector<float> propValue;
		Textrue* texturePtr;
		GLuint propLocation;

		template<typename... valueType>
		void SetPropValue(valueType ... value);

		~MaterialProperty();



};


