/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
melÇc++ì‡Ç≈é¿çsÇ∑ÇÈ

melExecute;

*/

#include <maya\MStatus.h>
#include <maya\MPxCommand.h>
#include <maya\MArgList.h>
#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>

class melExecute : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);\
	static void *creator() { return new melExecute; }

};


MStatus melExecute::doIt(const MArgList& args)
{
	MGlobal::executeCommand("delete (`ls -sl`);");
	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("melExecute", melExecute::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("melExecute");

	return MS::kSuccess;
}