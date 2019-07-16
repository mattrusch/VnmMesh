// VnmMesh

#pragma once

#include <cstdint>

namespace Vnm
{
	class MeshHeader
	{
	public:
		uint32_t mMagic;
		uint32_t mNumSubmeshes;
		uint32_t mNumTotalVertices;
		uint32_t mNumTotalIndices;
	};

	const uint32_t meshNameSize = 64;
	class SubmeshDesc
	{
	public:
		char	 mName[meshNameSize];
		uint32_t mNumVertices;
		uint32_t mNumIndices;
	};
}
