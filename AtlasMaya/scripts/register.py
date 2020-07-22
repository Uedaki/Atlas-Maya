import pymel.core as pm
import maya.cmds as mc

from rendering import (createRenderingMelProxy)

def register():
    pm.renderer("AtlasMaya", rendererUIName = "Atlas Renderer")

    pm.renderer("AtlasMaya", edit = True,
                renderProcedure = "AtlasRenderProcedureProxy")

    createRenderingMelProxy()
  

def unregister():
    pm.renderer('AtlasMaya', unregisterRenderer = True)