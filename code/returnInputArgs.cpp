/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
MArgList�͈������X�g���쐬����ю擾����ۂɎg�p���܂�

returnInputArgs "arg1" "arg2" "arg3";

Result:
// arg1 //
// arg3 //
*/

#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>

class returnInputArgs : public MPxCommand
{
public:
	MStatus doIt(const MArgList& args);
	static void* creator();
};

MStatus returnInputArgs::doIt(const MArgList& args)
{
	// args1��String�ɕϊ�����
	MString argsString0 = args.asString(0);
	MGlobal::displayInfo(argsString0);
	// args3��String�ɕϊ�����
	MString argsString2 = args.asString(2);
	MGlobal::displayInfo(argsString2);
	return MS::kSuccess;
}

void* returnInputArgs::creator()
{
	return new returnInputArgs;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("returnInputArgs", returnInputArgs::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("returnInputArgs");

	return MS::kSuccess;
}