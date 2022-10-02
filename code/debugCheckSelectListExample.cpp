/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
選択中のオブジェクトをprintDebugすることができる

debugCheckSelectListExample;;

Result:
// pCylinder1 //
// pSphere1 //
// pCube1 //
*/
#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MSelectionList.h>
#include <maya/MIOStream.h>
#include <vector>

DeclareSimpleCommand(debugCheckSelectListExample, "UnPySide", "1.0");

MStatus debugCheckSelectListExample::doIt(const MArgList&)
{
	MDagPath node;
	MObject component;
	MSelectionList list;
	MFnDagNode nodeFn;
	MGlobal::getActiveSelectionList(list);
	//std::vector<std::string> tmpList;

	for (unsigned int index = 0; index < list.length(); index++)
	{
		list.getDagPath(index, node, component);
		nodeFn.setObject(node);
		//cout << nodeFn.name().asChar() << " is selected" << endl;
		//tmpList.push_back(nodeFn.name().asChar());
		MGlobal::displayInfo(nodeFn.name().asChar());
	}
	return MS::kSuccess;
}