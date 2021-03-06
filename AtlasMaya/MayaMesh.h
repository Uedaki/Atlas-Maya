#pragma once

#include <Maya/MFnMesh.h>
#include <Maya/MDagPath.h>
#include <Maya/MFloatVectorArray.h>
#include <Maya/MIntArray.h>
#include <Maya/MMatrix.h>
#include <Maya/MPointArray.h>

#include <map>

#include "Atlas/Mesh.h"

#include "MayaShaderNetworkBuilder.h"
#include "Logger.h"

class MayaMesh : public atlas::Mesh
{
public:
	MayaMesh(MFnMesh &mMesh, MayaShaderNetworkBuilder &shaderBuilder)
	{
		MDagPath mDag;
		MMatrix mMatrix;
		if (mMesh.getPath(mDag) == MS::kSuccess)
			mMatrix = mDag.inclusiveMatrix();

		MPointArray mVertices;
		mMesh.getPoints(mVertices);
		vertices.resize(mVertices.length());
		MPoint mLowerBound = mVertices[0] * mMatrix;
		MPoint mUpperBound = mVertices[0] * mMatrix;
		for (unsigned int i = 0; i < mVertices.length(); i++)
		{
			MPoint point = mVertices[i] * mMatrix;
			vertices[i] = glm::vec3(point.x, point.y, point.z);

			mLowerBound.x = std::min(mLowerBound.x, point.x);
			mLowerBound.y = std::min(mLowerBound.y, point.y);
			mLowerBound.z = std::min(mLowerBound.z, point.z);

			mUpperBound.x = std::max(mUpperBound.x, point.x);
			mUpperBound.y = std::max(mUpperBound.y, point.y);
			mUpperBound.z = std::max(mUpperBound.z, point.z);
		}
		setBound(BoundType::LOWER_BOUND, glm::vec3(mLowerBound.x, mLowerBound.y, mLowerBound.z));
		setBound(BoundType::UPPER_BOUND, glm::vec3(mUpperBound.x, mUpperBound.y, mUpperBound.z));

		MFloatVectorArray mNormal;
		mMesh.getNormals(mNormal);
		for (unsigned int i = 0; i < mNormal.length(); ++i)
		{
			normals.emplace_back(mNormal[i].x, mNormal[i].y, mNormal[i].z);
		}

		MIntArray mTrianglesCount;
		MIntArray mVerticesIndice;
		mMesh.getTriangles(mTrianglesCount, mVerticesIndice);

		for (int polygon = 0; polygon < mMesh.numPolygons(); ++polygon)
		{
			MIntArray vertexList;
			MIntArray faceNormalList;
			mMesh.getPolygonVertices(polygon, vertexList);
			mMesh.getFaceNormalIds(polygon, faceNormalList);

			std::map<int, int> vertexToNormal;
			for (unsigned int i = 0; i < vertexList.length(); ++i)
			{
				vertexToNormal.emplace(vertexList[i], faceNormalList[i]);
			}

			for (int i = 0; i < mTrianglesCount[polygon]; ++i)
			{
				int vertexId[3];
				mMesh.getPolygonTriangleVertices(polygon, i, vertexId);
				indices.push_back({ vertexId[0], vertexToNormal[vertexId[0]],
					vertexId[1], vertexToNormal[vertexId[1]],
					vertexId[2], vertexToNormal[vertexId[2]] });
			}
		}

		LOG_INFO("Object %s recorded with %d triangles using %d vertices and %d normals", name, indices.size(), vertices.size(), normals.size());

		MObjectArray mShaderArray;
		MIntArray mShaderIndices;
		mMesh.getConnectedShaders(0, mShaderArray, mShaderIndices);
		for (uint32_t i = 0; i < mShaderArray.length(); i++)
		{
			if (i == 0)
				material = shaderBuilder.buildGraph(mShaderArray[i]);
			else
				shaderBuilder.buildGraph(mShaderArray[i]);
		}
	}
};