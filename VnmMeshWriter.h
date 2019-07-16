// VnmMeshWriter.h

#pragma once

namespace Vnm
{
	class ExportMesh;
	class MeshWriter
	{
	public:
		void WriteMesh(const ExportMesh& exportMesh, const char* outputFileName) const;
	};
}
