#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <GL\glew.h>

using namespace std;

class MaterialProperty
{
	public:
		MaterialProperty();
		MaterialProperty(string name, GLint type, GLuint location);

		string propName;
		GLint propType;
		vector<float> propValue;
		GLuint propLocation;

		template<typename... valueType>
		void SetPropValue(valueType ... value);

		~MaterialProperty();



};


