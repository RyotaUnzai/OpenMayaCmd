/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
MPlugを使ってノードの値を取る

pSphere*とつく名前のノードのリストを取得する
returnConpornentType;

Result:
// arg1 //
// arg3 //
*/

#include <maya\MStatus.h>
#include <maya\MPxCommand.h>
#include <maya\MArgList.h>
#include <maya\MDagPath.h>
#include <maya\MGlobal.h>
#include <maya\MObject.h>
#include <maya\MFnDependencyNode.h>
#include <maya\MSelectionList.h>
#include <maya\MFnSingleIndexedComponent.h>
#include <maya\MFnPlugin.h>

class returnConpornentType : public MPxCommand
{
public:
	MStatus doIt(const MArgList& args);
	static void* creator();
};

MStatus returnConpornentType::doIt(const MArgList& args)
{
	MSelectionList sList;
	MGlobal::getActiveSelectionList(sList);

	MObject obj;
	sList.getDependNode(0, obj);

	MObject component;
	MDagPath dagPath;

	sList.getDagPath(0, dagPath, component);

	char fulPathNameChar[100];
	char apiTypeStrChar[100];
	char elementCountChar[100];
	sprintf_s(fulPathNameChar, "fulPathName: %s", dagPath.fullPathName().asChar());
	/// オブジェクトのタイプの文字列を返します。
	sprintf_s(apiTypeStrChar, "apiTypeStrName: %s", component.apiTypeStr());
	sprintf_s(elementCountChar, "elementCount: %d", MFnSingleIndexedComponent(component).elementCount());
	MGlobal::displayInfo(fulPathNameChar);
	MGlobal::displayInfo(apiTypeStrChar);
	MGlobal::displayInfo(elementCountChar);


	return MS::kSuccess;
}

void* returnConpornentType::creator()
{
	return new returnConpornentType;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("returnConpornentType", returnConpornentType::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("returnConpornentType");

	return MS::kSuccess;
}