/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
�I�������e�I�u�W�F�N�g��vertex�̈ʒu���Ɋ�Â���vertex color��t����

postionVertexColor;

*/

#include <maya\MStatus.h>
#include <maya\MPxCommand.h>
#include <maya\MArgList.h>
#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>
#include <maya\MColorArray.h>
#include <maya\MColor.h>
#include <maya\MFnMesh.h>
#include <maya\MSelectionList.h>
#include <maya\MObject.h>
#include <maya\MDagPath.h>
#include <maya\MIntArray.h>
#include <maya\MFloatVectorArray.h>
#include <maya\MFnSingleIndexedComponent.h>
#include <maya\MFn.h>
#include <maya\MArrayDataHandle.h>
#include <maya\MDataHandle.h>

class postionVertexColor : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);\
	static void *creator() { return new postionVertexColor; }
};



MStatus postionVertexColor::doIt(const MArgList& args)
{
	MSelectionList selList;
	MGlobal::getActiveSelectionList(selList);

	for (unsigned int count = 0; count < selList.length(); count++) {
		MDagPath dagPath;
		selList.getDagPath(count, dagPath);
		MFnMesh meshFn(dagPath);
		MFnDependencyNode dnFn = dagPath.node();
		MColorArray vertexColorList;
		MFloatVectorArray normalsList;
		// meshFn��colorSet�̂��ׂĂ̒��_�̐F�̔z���vertexColorList�ɃR�s�[
		meshFn.getVertexColors(vertexColorList);
		// meshFn�̃��b�V���@�����X�g��normalsList�ɔz����R�s�[
		meshFn.getNormals(normalsList);
		// meshFn��vertexColorList�̒���
		int LenVertexList = vertexColorList.length();
		MFnSingleIndexedComponent fnComponent;
		MFnSingleIndexedComponent fullComponent = fnComponent.create(MFn::kMeshVertComponent);
		fnComponent.setCompleteData(LenVertexList);
		// �����f�[�^�^�̔z��
		MIntArray vertexIndexList;
		// setCompleteData�őI�������v�f�̑������炷�ׂĂ̗v�f�C���f�b�N�X��vertexIndexList�ɃR�s�[����
		fnComponent.getElements(vertexIndexList);

		for (unsigned int index = 0; index < vertexIndexList.length(); index++) {
			vertexColorList[index].r = normalsList[index].x;
			vertexColorList[index].g = normalsList[index].y;
			vertexColorList[index].b = normalsList[index].z;
			vertexColorList[index].a = 0.5f;
		}
		//meshFn.setVertexColors(vertexColorList, vertexIndexList, NULL);
		meshFn.setVertexColors(vertexColorList, vertexIndexList, 0, MFnMesh::MColorRepresentation::kRGB);
		//meshFn.setVertexColors(vertexColorList, vertexIndexList, 0, MFnMesh::MColorRepresentation::kRGBA);
	}
	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("postionVertexColor", postionVertexColor::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("postionVertexColor");

	return MS::kSuccess;
}