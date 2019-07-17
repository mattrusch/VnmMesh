// VnmMeshWriter.h

#pragma once

namespace Vnm
{
	class ExportMesh;
	class MeshWriter
	{
	public:
		void WriteMesh(const char* outputFileName, const ExportMesh& exportMesh) const;
	};
}
