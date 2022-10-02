/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
選択した各オブジェクトに任意のvertex colorを付ける

setVertexColor;

*/

#include <maya\MPxCommand.h>
#include <maya\MStatus.h>
#include <maya\MArgList.h>
#include <maya\MObject.h>
#include <maya\MFnPlugin.h>
#include <maya\MSelectionList.h>
#include <maya\MGlobal.h>
#include <maya\MItSelectionList.h>
#include <maya\MDagPath.h>

class getDAGPathName : public MPxCommand {
public:
	MStatus doIt(const MArgList& args);
	static void* creator();
};

MStatus getDAGPathName::doIt(const MArgList& args) {
	MSelectionList sel;
	MGlobal::getActiveSelectionList(sel);
	MItSelectionList iter(sel);
	MDagPath dagPath;
	char nameChar[300];
	for (; !iter.isDone(); iter.next()) {
		iter.getDagPath(dagPath);
		dagPath.extendToShape();
		sprintf_s(nameChar, "name: %s", dagPath.fullPathName().asChar());
		MGlobal::displayInfo(nameChar);
	}
	return MS::kSuccess;
}

void* getDAGPathName::creator() {
	return new getDAGPathName;
}

MStatus initializePlugin(MObject obj) {
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("getDAGPathName", getDAGPathName::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj) {
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("getDAGPathName");
	return MS::kSuccess;
}