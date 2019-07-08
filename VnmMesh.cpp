// VnmMesh.cpp

#include <fbxsdk.h>
#include <cstdint>
#include <vector>
#include <string>

namespace Vnm
{
	void Log(const char* message)
	{
		printf("%s\n", message);
	}

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

	class VnmExportMesh
	{
	public:
		std::vector<std::string> mNames;
		std::vector<uint32_t>    mNumIndices;
		std::vector<uint32_t>    mNumVertices;
		std::vector<Vertex>      mVertices;
		std::vector<IndexType>   mIndices;
	};

	class FbxLoader
	{
	public:
		FbxLoader() = default;
		~FbxLoader() = default;

		void Create();
		void ImportFile(const char* filename);
		void BuildMeshPreorder(fbxsdk::FbxNode* node, VnmExportMesh& exportMesh);

	protected:
		fbxsdk::FbxManager*  mFbxManager;
		fbxsdk::FbxImporter* mFbxImporter;
		fbxsdk::FbxScene*	 mFbxScene;
	};

	void FbxLoader::Create()
	{
		mFbxManager = fbxsdk::FbxManager::Create();
		auto ioSettings = fbxsdk::FbxIOSettings::Create(mFbxManager, IOSROOT);
		mFbxManager->SetIOSettings(ioSettings);

		mFbxImporter = fbxsdk::FbxImporter::Create(mFbxManager, "VnmImporter");

		mFbxScene = fbxsdk::FbxScene::Create(mFbxManager, "VnmScene");
	}

	void FbxLoader::ImportFile(const char* filename)
	{
		int fileFormat = -1;
		bool result = mFbxImporter->Initialize(filename, fileFormat, mFbxManager->GetIOSettings());
		if (!result)
		{
			Log("Failed to initialize FBX importer");
			return;
		}

		result = mFbxImporter->Import(mFbxScene);
		if (!result)
		{
			Log("Failed to load FBX file");
			return;
		}
	}

	static void AppendMesh(fbxsdk::FbxMesh* fbxMesh, VnmExportMesh& exportMesh)
	{
		exportMesh.mNumVertices.emplace_back(fbxMesh->GetPolygonVertexCount());
	}

	void FbxLoader::BuildMeshPreorder(fbxsdk::FbxNode* node, VnmExportMesh& exportMesh)
	{
		FbxMesh* fbxMesh = node->GetMesh();
		if (fbxMesh != nullptr)
		{
		}

		int numChildren = node->GetChildCount();
		for (int i = 0; i < numChildren; ++i)
		{
			BuildMeshPreorder(node->GetChild(i), exportMesh);
		}
	}
}

int main()
{
    return 0;
}
