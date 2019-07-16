// VnmExportMesh.h

#pragma once

#include <vector>

namespace Vnm
{
	class Vector4
	{
	public:
		float v[4];
	};

	class Vector3
	{
	public:
		float v[3];
	};

	class Vector2
	{
	public:
		float v[2];
	};

	class Vertex
	{
	public:
		Vector3 mPosition;
		Vector3 mNormal;
		Vector4 mTangent;	// w component stores handedness
		Vector2 mUv;
	};

	using IndexType = uint32_t;

	class ExportMesh
	{
	public:
		std::vector<std::string> mNames;
		std::vector<uint32_t>    mNumIndices;
		std::vector<uint32_t>    mNumVertices;
		std::vector<Vertex>      mVertices;
		std::vector<IndexType>   mIndices;
	};
}
