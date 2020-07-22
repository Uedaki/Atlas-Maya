import maya.mel as mel

def createRenderingMelProxy():
    mel.eval('''
        global proc AtlasRenderProcedureProxy(int $width, int $height, int $doShadows, int $doGlowPass, string $camera, string $option)
        {
            AtlasRenderProcedure -w $width -h $height -c $camera;
        }''')

    #mel.eval('''
    #    global proc customMayaRendererRenderRegionProcedureProxy(int $width, int $height, int $doShadows, int $doGlowPass, string $camera, string $option)
    #    {
    #        python("import CustomMayaRenderer.globalsNode");
    #        python("CustomMayaRenderer.globalsNode.createGlobalsNode()");
    #        customMayaRendererRenderRegionProcedure -w $width -h $height -c $camera;
    #    }''')