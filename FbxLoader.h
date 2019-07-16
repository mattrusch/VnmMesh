// FbxLoader.h

#pragma once

#include <fbxsdk.h>

namespace Vnm
{
	class ExportMesh;

	class FbxLoader
	{
	public:
		FbxLoader() = default;
		~FbxLoader() = default;

		void Create();
		void ImportFile(const char* filename);
		void BuildMesh(ExportMesh& exportMesh);

	protected:
		fbxsdk::FbxManager*  mFbxManager;
		fbxsdk::FbxImporter* mFbxImporter;
		fbxsdk::FbxScene*	 mFbxScene;
	};
}
