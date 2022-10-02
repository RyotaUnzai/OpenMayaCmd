/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
FlipしたUVのみ選択する

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

	// 選択しているオブジェクトをリストにする
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);
	// 選択しているオブジェクトがない場合は処理を終了する
	if (selection.length() == 0)
		return MS::kFailure;

	// MltSelectionList でセレクション リスト全体に繰り返し処理する
	MItSelectionList iter(selection);
	MSelectionList result;


	MString colorSetName = "colorSet2";
	// イテレータが完了した状態になるまで繰り返すnext()で無限ループを回避する
	while (! iter.isDone()) {
		// Mesh情報を取得する準備と、フェイス単位の処理をする準備
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


		
		// face単位の繰り返し処理
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