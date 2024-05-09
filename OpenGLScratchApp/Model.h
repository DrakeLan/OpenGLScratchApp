#pragma once

#include <vector>
#include <string>

#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Textrue.h"

class Model
{
public:
	Model();
	Model(bool tangentBool);

	void LoadModel(const std::string& fileName);
	std::vector<Mesh*> GetMeshList();
	void RenderModel();
	void RenderModelInstancing(GLsizei instancingCount);
	void clearModel();

	~Model();

private:

	bool tangentFlag = false;

	void LoadNode(aiNode *node, const aiScene *scene);
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterial(const aiScene *scene);

	std::vector<Mesh*> meshList;
	std::vector<Textrue*> textureList;
	std::vector<unsigned int> meshToTex;

};

