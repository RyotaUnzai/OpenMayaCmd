/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
pythonÇc++ì‡Ç≈é¿çsÇ∑ÇÈ

pythonExecute;

*/

#include <maya\MStatus.h>
#include <maya\MPxCommand.h>
#include <maya\MArgList.h>
#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>

class pythonExecute : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);\
	static void *creator() { return new pythonExecute; }

};


MStatus pythonExecute::doIt(const MArgList& args)
{
	MGlobal::executePythonCommand("maya.cmds.delete(maya.cmds.ls(sl = 1))");
	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("pythonExecute", pythonExecute::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("pythonExecute");

	return MS::kSuccess;
}