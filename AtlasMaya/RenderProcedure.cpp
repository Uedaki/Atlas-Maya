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
#include "MayaNodeId.h"
#include "MayaAreaLight.h"

#include "Logger.h"

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
	LOG_DEBUG("Executing command " << __FUNCTION__);
	try
	{
		int width = 1;
		int height = 1;
		MString mCameraName;

		MArgDatabase argsData(createSyntax(), args);
		if (!argsData.isFlagSet("-width")
			|| !argsData.isFlagSet("-height")
			|| !argsData.isFlagSet("-camera"))
		{
			LOG_WARNING("Wrong number of argument passed to render procedure");
			return (MS::kFailure);
		}

		argsData.getFlagArgument("-width", 0, width);
		argsData.getFlagArgument("-height", 0, height);
		argsData.getFlagArgument("-camera", 0, mCameraName);

		LOG_DEBUG("width: " << width << " height: " << height << " camera: " << mCameraName);

		atlas::ShaderNetwork network;
		MayaShaderNetworkBuilder networkBuilder(network);

		for (MItDag it; !it.isDone(); it.next())
		{
			MObject obj = it.currentItem();


			LOG_DEBUG("Object: " << obj.apiTypeStr());
			if (obj.apiType() == MFn::kMesh)
			{
				MFnMesh mesh(obj);
				LOG_DEBUG("- name: " << mesh.name());

				MObjectArray mShaderArray;
				MIntArray mShaderIndices;
				mesh.getConnectedShaders(0, mShaderArray, mShaderIndices);
				//	std::cout << "nbr face " << mesh.numPolygons() << " nbr indice " << mShaderIndices.length() << std::endl;
				for (uint32_t i = 0; i < mShaderArray.length(); i++)
				{
					size_t gra = networkBuilder.buildGraph(mShaderArray[i]);
					glm::vec3 c = network.sampleGraph(gra);
					LOG_DEBUG("output color (" << c.x << ", " << c.y << ", " << c.z << ")");

				}
			}
			else
			{
				MFnDependencyNode itNode(obj);

				if (itNode.typeId().id() == MayaNodeId::AREA_LIGHT)
				{
					atlas::AreaLight l;
					AreaLightNode::fetchAttribute(itNode, l);
				}
			}
		}
	}
	catch (Exception &e)
	{
		LOG_EXCEPTION(e);
		return (MS::kFailure);
	}

	return (MS::kSuccess);
}