// VnmMesh.cpp

#include "FbxLoader.h"
#include "VnmExportMesh.h"
#include "VnmMeshUtil.h"
#include "VnmMeshWriter.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		Vnm::Log("Usage: VnmMesh [input filename] [output filename]");
		return -1;
	}

	const char* meshFilename = argv[1];
	const char* outputFilename = argv[2];

	Vnm::FbxLoader fbxLoader;
	fbxLoader.Create();
	fbxLoader.ImportFile(meshFilename);

	Vnm::ExportMesh exportMesh;
	fbxLoader.BuildMesh(exportMesh);

	Vnm::MeshWriter meshWriter;
	meshWriter.WriteMesh(outputFilename, exportMesh);

    return 0;
}
