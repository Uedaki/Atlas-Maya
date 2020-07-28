import maya.cmds as mc
import maya.mel as mel

atlasMenu = None

def createLocator(locatorType):
    xformName = mc.createNode("transform", name=locatorType + "1")
    shapeName = xformName.replace(locatorType, locatorType + "Shape")
    return (xformName, mc.createNode(locatorType, name=shapeName, parent=xformName))

def createAreaLight():
    print("Create area light")

    (xform, shape) = createLocator('AtlasAreaLight')
     #Add the locator to the light set.
    mc.connectAttr(
        xform + '.instObjGroups',
        'defaultLightSet.dagSetMembers',
        nextAvailable=True)

def createMenu():
    global atlasMenu
    deleteMenu()

    mainWindow = mel.eval('$temp1=$gMainWindow')
    atlasMenu = mc.menu("AtlasMenu", parent = mainWindow, label = "Atlas", tearOff = True)

    mc.menuItem(
        "AtlasLightMenu",
        subMenu=True,
        label="Lights",
        to=True,
        parent="AtlasMenu")

    mc.menuItem(
        label="Create area Light",
        parent="AtlasLightMenu",
        command = "import AtlasMaya.menu\nAtlasMaya.menu.createAreaLight()")

    mc.menuItem(divider=True, parent="AtlasMenu")

    mc.menuItem(
        "AtlasDebugMenu",
        subMenu=True,
        label="Debug",
        to=True,
        parent="AtlasMenu")

    mc.menuItem(
        label = "Enable profiler",
        parent = "AtlasDebugMenu",
        checkBox = True)


def deleteMenu():
    global atlasMenu
    try:
        mc.deleteUI(atlasMenu)
    except:
        pass