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
		GLuint propLocation;
		bool dirtyFlag;
		
		void SetPropValue(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);

		~MaterialProperty();

};



