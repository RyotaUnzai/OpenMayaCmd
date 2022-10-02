/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
MFnMesh���g�p����NumPolygons���擾����

�I�u�W�F�N�g��I�����Ď��s
returnGetMFnMeshNumPolygons;

Result:
// num of polygons: 6 //
*/

#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya\MSelectionList.h>
#include <maya\MGlobal.h>
#include <maya\MItSelectionList.h>
#include <maya\MDagPath.h>
#include <maya\MFnDependencyNode.h>
#include <maya\MPlug.h>
#include <maya\MString.h>
#include <maya\MFnMesh.h>

class returnGetMFnMeshNumPolygons : public MPxCommand
{
public:
	MStatus doIt(const MArgList& args);
	static void* creator();
};

MStatus returnGetMFnMeshNumPolygons::doIt(const MArgList& args)
{

	//�I�����X�g���擾���A�C�e���[�^�[���g�p���đI�����X�g���g���[�X����
	MSelectionList selList;
	MGlobal::getActiveSelectionList(selList);
	MDagPath dagPath;

	/*
	�I�����Ă���Transform�m�[�h�̉��ɂ���Mesh Shape��path���擾��
	extendToShape()��dagPath�̒��g��mesh��path�֕ύX����
	*/
	selList.getDagPath(0, dagPath);
	dagPath.extendToShape();

	//�v���O�����o����p��MFnDependencyNode���쐬
	MFnDependencyNode dnFn = dagPath.node();
	// findPlug�֐��Ń��b�V���f�[�^�̃A�g���r���[�g�����w�肷��
	MPlug plug = dnFn.findPlug("outMesh", false);

	/*
	�v���O�̒l��MObject�Ƃ��Ď擾
	MFnMesh�ɓn���āAMFnMesh�o�R�Ń��b�V���f�[�^�����o���B
	*/
	MObject obj = plug.asMObject();
	MFnMesh meshFn(obj);

	char meshFnChar[100];
	sprintf_s(meshFnChar, "num of polygons: %d", meshFn.numPolygons());
	MGlobal::displayInfo(meshFnChar);

	return MS::kSuccess;
}

void* returnGetMFnMeshNumPolygons::creator()
{
	return new returnGetMFnMeshNumPolygons;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("returnGetMFnMeshNumPolygons", returnGetMFnMeshNumPolygons::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("returnGetMFnMeshNumPolygons");

	return MS::kSuccess;
}