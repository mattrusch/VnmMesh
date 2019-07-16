// VnmMesh.cpp

#include "FbxLoader.h"
#include "VnmExportMesh.h"
#include "VnmMeshUtil.h"

int main(int argc, char* argv[])
{
	if (argc < 1)
	{
		Vnm::Log("Usage: VnmMesh [filename]");
		return -1;
	}

	const char* meshFilename = argv[1];

	Vnm::FbxLoader fbxLoader;
	fbxLoader.Create();
	fbxLoader.ImportFile(meshFilename);

	Vnm::ExportMesh exportMesh;
	fbxLoader.BuildMesh(exportMesh);

    return 0;
}
