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

	void MeshWriter::WriteMesh(const char* outputFileName, const ExportMesh& exportMesh) const
	{
		if (!ValidExportMesh(exportMesh))
		{
			Log("Invalid export mesh\n");
			return;
		}

		const uint32_t vnmMesh4cc = ('m' << 24) | ('m' << 16) | ('n' << 8) | ('v');
		MeshHeader header;
		header.mMagic = vnmMesh4cc;
		header.mNumSubmeshes = static_cast<uint32_t>(exportMesh.mNumVertices.size());
		header.mNumTotalVertices = static_cast<uint32_t>(exportMesh.mVertices.size());
		header.mNumTotalIndices = static_cast<uint32_t>(exportMesh.mIndices.size());

		std::vector<SubmeshDesc> submeshDescs(header.mNumSubmeshes);
		for (uint32_t i = 0; i < header.mNumSubmeshes; ++i)
		{
			strncpy_s(submeshDescs[i].mName, exportMesh.mNames[i].c_str(), sizeof(submeshDescs[i].mName));
			submeshDescs[i].mNumVertices = exportMesh.mNumVertices[i];
			submeshDescs[i].mNumIndices = exportMesh.mNumIndices[i];
		}

		std::fstream meshFile(outputFileName, std::ios::out | std::ios::binary);
		if (!meshFile.is_open())
		{
			Log("Cannot open output file\n");
			return;
		}

		meshFile.write(reinterpret_cast<char*>(&header), sizeof(header));
		meshFile.write(reinterpret_cast<char*>(submeshDescs.data()), submeshDescs.size() * sizeof(submeshDescs[0]));
		meshFile.write(reinterpret_cast<const char*>(exportMesh.mVertices.data()), exportMesh.mVertices.size() * sizeof(exportMesh.mVertices[0]));
		meshFile.write(reinterpret_cast<const char*>(exportMesh.mIndices.data()), exportMesh.mIndices.size() * sizeof(exportMesh.mIndices[0]));
		meshFile.close();
	}
}
