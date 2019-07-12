// VnmMesh.cpp

#include <fbxsdk.h>
#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cassert>

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
		void BuildMesh(VnmExportMesh& exportMesh);

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
		int numVerts = 0;
		int numIndices = 0;

		auto vertexPositions = fbxMesh->GetControlPoints();
		int polygonCount = fbxMesh->GetPolygonCount();

		const int numTriangleVertices = 3;
		assert(fbxMesh->GetPolygonSize(0) == numTriangleVertices && "Mesh needs to be triangulated");
		
		for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
		{
			auto positions = fbxMesh->GetControlPoints();
			for (int triVertIndex = 0; triVertIndex < numTriangleVertices; ++triVertIndex)
			{
				Vertex exportVertex;

				int vertexIndex = fbxMesh->GetPolygonVertex(polygonIndex, triVertIndex);
				exportVertex.mPosition.v[0] = static_cast<float>(vertexPositions[vertexIndex].mData[0]);
				exportVertex.mPosition.v[1] = static_cast<float>(vertexPositions[vertexIndex].mData[1]);
				exportVertex.mPosition.v[2] = static_cast<float>(vertexPositions[vertexIndex].mData[2]);

				exportMesh.mVertices.emplace_back(exportVertex);
				exportMesh.mIndices.emplace_back(numVerts++);
				++numIndices;
			}
		}

		exportMesh.mNames.emplace_back(fbxMesh->GetName());
		exportMesh.mNumVertices.emplace_back(numVerts);
		exportMesh.mNumIndices.emplace_back(numIndices);
	}

	void BuildMeshPreorder(fbxsdk::FbxNode* node, VnmExportMesh& exportMesh)
	{
		FbxMesh* fbxMesh = node->GetMesh();
		if (fbxMesh != nullptr)
		{
			AppendMesh(fbxMesh, exportMesh);
		}

		int numChildren = node->GetChildCount();
		for (int i = 0; i < numChildren; ++i)
		{
			BuildMeshPreorder(node->GetChild(i), exportMesh);
		}
	}

	void FbxLoader::BuildMesh(VnmExportMesh& exportMesh)
	{
		fbxsdk::FbxGeometryConverter geometryConverter(mFbxManager);
		geometryConverter.Triangulate(mFbxScene, true);

		BuildMeshPreorder(mFbxScene->GetRootNode(), exportMesh);
	}
}

int main()
{
	Vnm::FbxLoader fbxLoader;
	fbxLoader.Create();
	fbxLoader.ImportFile("box.fbx");

	Vnm::VnmExportMesh exportMesh;
	fbxLoader.BuildMesh(exportMesh);

    return 0;
}
