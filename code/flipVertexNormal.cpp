/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
Flip����UV�̂ݑI������

selectFlipUV;

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
#include <maya\MFnMesh.h>
#include <maya\MItMeshPolygon.h>
#include <maya\MFnDependencyNode.h>
#include <maya\MDagPath.h>
#include <maya\MTypes.h>

class selectFlipUV : public MPxCommand {
public:
	MStatus doIt(const MArgList& args);
	static void* creator();
private:
};

void MFloatVectorCross(MFloatVector v1, MFloatVector v2, MFloatVector &result) {
	MFloatVector v(
		(v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x)
	);
	result = v;
}

MStatus selectFlipUV::doIt(const MArgList& args) {
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);
	if (selection.length() == 0)
		return MS::kFailure;

	MDagPath dagpath;
	selection.getDagPath(0, dagpath);
	MVectorArray normals;
	MIntArray vertexList;
	MFnMesh fnMesh(dagpath);
	MItMeshPolygon iter(dagpath);
	while (!iter.isDone()) {
		int polyId = iter.index();
		fnMesh.getPolygonVertices(polyId, vertexList);
		iter.getNormals(normals, MSpace::kObject);

		for (int i = 0; i < normals.length(); i++) {
			int vertexId = vertexList[i];
			MVector reversNormal = normals[i] * -1;
			fnMesh.setFaceVertexNormal(reversNormal, polyId, vertexId, MSpace::kObject);
		}
		iter.next();
	}

	/*

	// �I�����Ă���I�u�W�F�N�g�����X�g�ɂ���
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);
	// �I�����Ă���I�u�W�F�N�g���Ȃ��ꍇ�͏������I������
	if (selection.length() == 0)
		return MS::kFailure;

	// MltSelectionList �ŃZ���N�V���� ���X�g�S�̂ɌJ��Ԃ���������
	MItSelectionList iter(selection);
	MSelectionList result;


	MString colorSetName = "colorSet2";
	// �C�e���[�^������������ԂɂȂ�܂ŌJ��Ԃ�next()�Ŗ������[�v���������
	while (! iter.isDone()) {
		// Mesh�����擾���鏀���ƁA�t�F�C�X�P�ʂ̏��������鏀��
		MDagPath dagPath;
		MObject component;
		iter.getDagPath(dagPath, component);
		MFnMesh fnMesh(dagPath);
		MItMeshPolygon polyIter(dagPath, component);
		MColorArray colors;
		int count = 0;
		while (fnMesh.numVertices() == count)
		{
			colors.append(MColor(0, 0, 1));
			count++;
		}
		//fnMesh.setSomeColors(count, colors, colorSetName);


		
		// face�P�ʂ̌J��Ԃ�����
		while (!polyIter.isDone()) {
			//fnMesh.setSomeColors(count, colors, colorSetName);
			polyIter.next();
		}
		iter.next();
	}*/
	return MS::kSuccess;
}


void* selectFlipUV::creator() {
	return new selectFlipUV;
}

MStatus initializePlugin(MObject obj) {
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("selectFlipUV", selectFlipUV::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj) {
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("selectFlipUV");
	return MS::kSuccess;
}