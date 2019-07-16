// VnmMeshWriter.cpp

#include "VnmMeshWriter.h"
#include "VnmMesh.h"
#include "VnmExportMesh.h"
#include "VnmMeshUtil.h"
#include <fstream>
#include <vector>

namespace Vnm
{
	static bool ValidExportMesh(const ExportMesh& exportMesh)
	{
		if (!(exportMesh.mNames.size() == exportMesh.mNumIndices.size() &&
			  exportMesh.mNames.size() == exportMesh.mNumVertices.size()))
		{
			return false;
		}

		return true;
	}
	void MeshWriter::WriteMesh(const ExportMesh& exportMesh, const char* outputFileName) const
	{
		if (!ValidExportMesh(exportMesh))
		{
			Log("Invalid export mesh\n");
			return;
		}

		MeshHeader header;
		header.mMagic = ('v' << 24) | ('n' << 16) | ('m' << 8) | ('m');
		header.mNumSubmeshes = exportMesh.mNumVertices.size();
		header.mNumTotalVertices = exportMesh.mVertices.size();
		header.mNumTotalIndices = exportMesh.mIndices.size();

		std::vector<SubmeshDesc> submeshDescs(header.mNumSubmeshes);
		for (int i = 0; i < header.mNumSubmeshes; ++i)
		{
			strncpy(submeshDescs[i].mName, exportMesh.mNames[i].c_str(), sizeof(submeshDescs[i].mName));
			submeshDescs[i].mNumVertices = exportMesh.mNumVertices[i];
			submeshDescs[i].mNumIndices = exportMesh.mNumIndices[i];
		}
	}
}
