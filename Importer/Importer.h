#pragma once
#include <string>
#include "DataTypes.h"

namespace Importer
{
	void LoadScene(std::string path);
	std::vector<Mesh> GetMeshes();
}