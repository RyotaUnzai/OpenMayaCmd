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
#include <maya\MFnMesh.h>
#include <maya\MItMeshPolygon.h>
#include <maya\MFnDependencyNode.h>
#include <maya\MDagPath.h>

class getDAGPathName : public MPxCommand {
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

MStatus getDAGPathName::doIt(const MArgList& args) {
	// 選択しているオブジェクトをリストにする
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);
	// 選択しているオブジェクトがない場合は処理を終了する
	if (selection.length() == 0)
		return MS::kFailure;

	// MltSelectionList でセレクション リスト全体に繰り返し処理する
	MItSelectionList iter(selection);
	MSelectionList result;

	// イテレータが完了した状態になるまで繰り返すnext()で無限ループを回避する
	while (! iter.isDone()) {
		// Mesh情報を取得する準備と、フェイス単位の処理をする準備
		MDagPath dagPath;
		MObject component;
		iter.getDagPath(dagPath, component);
		MFnMesh fnMesh(dagPath);
		MItMeshPolygon polyIter(dagPath, component);

		// face単位の繰り返し処理
		while (!polyIter.isDone()) {
			// normalを取得する
			MVector normal;
			fnMesh.getPolygonNormal(polyIter.index(), normal);

			/* Tangentの取得
			Tangentは頂点の数だけ情報があるので「MFloatVectorArray」のインスタンスを用意
			必要なのは1つだけなので、最初の要素だけを取得する
			*/
			MFloatVectorArray tangents;
			fnMesh.getFaceVertexTangents(polyIter.index(), tangents);
			MFloatVector tangent = tangents[0];

			/*Binormalの取得
			Tangentと同様に頂点の数だけ情報があるので、同様に最初の要素だけを取得する
			*/
			MFloatVectorArray binormals;
			fnMesh.getFaceVertexBinormals(polyIter.index(), binormals);
			MFloatVector binormal = binormals[0];

			//TangentとBinormalの外積を求める
			MFloatVector cross;
			MFloatVectorCross(tangent, binormal, cross);
			float dot = MFloatVector(normal) * cross;

			// 法線と内積を求めて表裏を判断
			if (dot < 0.0){
				result.add(dagPath, polyIter.currentItem());
			}
			polyIter.next();
		}
		iter.next();
	}
	// MGlobal::setActiveSelectionListにリストを渡し、裏面のみの選択をする
	MGlobal::setActiveSelectionList(result);

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
