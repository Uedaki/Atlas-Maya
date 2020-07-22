#include "pch.h"
#include "RenderProcedure.h"

#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <Maya/MItDag.h>
#include <Maya/MFnMesh.h>
#include <maya/MIOStream.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>

#include "Atlas/ShaderNetwork.h"
#include "MayaShaderNetworkBuilder.h"

const char *RenderProcedure::name = "AtlasRenderProcedure";

void *RenderProcedure::creator()
{
	return (new RenderProcedure());
}

MSyntax RenderProcedure::createSyntax()
{
	MSyntax syntax;
	syntax.addFlag("-w", "-width", MSyntax::kLong);
	syntax.addFlag("-h", "-height", MSyntax::kLong);
	syntax.addFlag("-c", "-camera", MSyntax::kString);
	return (syntax);
}


MStatus RenderProcedure::doIt(const MArgList &args)
{
	std::cout << "RenderProcedure::doIt" << std::endl;
	
	int width = 1;
	int height = 1;
	MString mCameraName;

	MArgDatabase argsData(createSyntax(), args);
	if (!argsData.isFlagSet("-width")
		|| !argsData.isFlagSet("-height")
		|| !argsData.isFlagSet("-camera"))
	{
		std::cout << "Wrong number of argument passed to render procedure" << std::endl;
		return (MS::kFailure);
	}

	argsData.getFlagArgument("-width", 0, width);
	argsData.getFlagArgument("-height", 0, height);
	argsData.getFlagArgument("-camera", 0, mCameraName);

	std::cout << "width: " << width << " height: " << height << " camera: " << mCameraName << std::endl;

	atlas::ShaderNetwork network;
	MayaShaderNetworkBuilder networkBuilder(network);

	for (MItDag it; !it.isDone(); it.next())
	{
		MObject obj = it.currentItem();
		
		std::cout << "Object: " << obj.apiTypeStr() << std::endl;
		if (obj.apiType() == MFn::kMesh)
		{
			MFnMesh mesh(obj);
			std::cout << "- name: " << mesh.name() << std::endl;

			MObjectArray mShaderArray;
			MIntArray mShaderIndices;
			mesh.getConnectedShaders(0, mShaderArray, mShaderIndices);
		//	std::cout << "nbr face " << mesh.numPolygons() << " nbr indice " << mShaderIndices.length() << std::endl;
			for (uint32_t i = 0; i < mShaderArray.length(); i++)
			{
				size_t gra = networkBuilder.buildGraph(mShaderArray[i]);
				glm::vec3 c = network.sampleGraph(gra);
				std::cout << "output color (" << c.x << ", " << c.y << ", " << c.z << ")" << std::endl;

			}
		}
	}

	return (MS::kSuccess);
}