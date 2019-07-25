// FbxLoader.cpp

#include "FbxLoader.h"
#include "VnmMeshUtil.h"
#include "VnmExportMesh.h"
#include <cassert>

namespace Vnm
{
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

	static void AppendMesh(fbxsdk::FbxMesh* fbxMesh, ExportMesh& exportMesh)
	{
		int numVerts = 0;
		int numIndices = 0;

		auto vertexPositions = fbxMesh->GetControlPoints();
		int polygonCount = fbxMesh->GetPolygonCount();

		const int numTriangleVertices = 3;
		assert(fbxMesh->GetPolygonSize(0) == numTriangleVertices && "Mesh needs to be triangulated");

		for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
		{
			fbxsdk::FbxVector4* positions = fbxMesh->GetControlPoints();
			for (int triVertIndex = 0; triVertIndex < numTriangleVertices; ++triVertIndex)
			{
				Vertex exportVertex;

				int vertexIndex = fbxMesh->GetPolygonVertex(polygonIndex, triVertIndex);
				exportVertex.mPosition.v[0] = static_cast<float>(vertexPositions[vertexIndex].mData[0]);
				exportVertex.mPosition.v[1] = static_cast<float>(vertexPositions[vertexIndex].mData[1]);
				exportVertex.mPosition.v[2] = static_cast<float>(vertexPositions[vertexIndex].mData[2]);

				fbxsdk::FbxVector4 normal;
				bool hasNormal = fbxMesh->GetPolygonVertexNormal(polygonIndex, triVertIndex, normal);
				if (!hasNormal)
				{
					normal = fbxsdk::FbxVector4(0.0, 1.0, 0.0);
				}
				exportVertex.mNormal.v[0] = static_cast<float>(normal.mData[0]);
				exportVertex.mNormal.v[1] = static_cast<float>(normal.mData[1]);
				exportVertex.mNormal.v[2] = static_cast<float>(normal.mData[2]);

				// Only use layer 0 uv and tangent basis data
				const fbxsdk::FbxLayerElementUV* layerUvs = fbxMesh->GetLayer(0)->GetUVs();
				fbxsdk::FbxVector2 uv;
				switch (layerUvs->GetMappingMode())
				{
				case FbxLayerElement::eByControlPoint:
					switch (layerUvs->GetReferenceMode())
					{
					case FbxLayerElement::eDirect:
						uv = layerUvs->GetDirectArray().GetAt(vertexIndex);
						break;
					case FbxLayerElement::eIndex:
					case FbxLayerElement::eIndexToDirect:
						int uvIndex = layerUvs->GetIndexArray().GetAt(vertexIndex);
						uv = layerUvs->GetDirectArray().GetAt(uvIndex);
						break;
					}
					break;
				case FbxLayerElement::eByPolygonVertex:
					int polyVertIndex = polygonIndex * numTriangleVertices + triVertIndex;
					switch (layerUvs->GetReferenceMode())
					{
					case FbxLayerElement::eDirect:
						uv = layerUvs->GetDirectArray().GetAt(polyVertIndex);
						break;
					case FbxLayerElement::eIndex:
					case FbxLayerElement::eIndexToDirect:
						int uvIndex = layerUvs->GetIndexArray().GetAt(polyVertIndex);
						uv = layerUvs->GetDirectArray().GetAt(uvIndex);
						break;
					}
					break;
				}

				exportVertex.mUv.v[0] = static_cast<float>(uv.mData[0]);
				exportVertex.mUv.v[1] = static_cast<float>(uv.mData[1]);

				//fbxsdk::FbxLayerElementTangent* layerTangents = fbxMesh->GetLayer(0)->GetTangents();

				exportMesh.mVertices.emplace_back(exportVertex);
				exportMesh.mIndices.emplace_back(numVerts++);
				++numIndices;
			}
		}

		exportMesh.mNames.emplace_back(fbxMesh->GetName());
		exportMesh.mNumVertices.emplace_back(numVerts);
		exportMesh.mNumIndices.emplace_back(numIndices);
	}

	static void BuildMeshPreorder(fbxsdk::FbxNode* node, ExportMesh& exportMesh)
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

	void FbxLoader::BuildMesh(ExportMesh& exportMesh)
	{
		fbxsdk::FbxGeometryConverter geometryConverter(mFbxManager);
		geometryConverter.Triangulate(mFbxScene, true);

		BuildMeshPreorder(mFbxScene->GetRootNode(), exportMesh);
	}
}
