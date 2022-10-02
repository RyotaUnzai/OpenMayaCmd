/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
選択した各オブジェクトのvertexの位置情報に基づいてvertex colorを付ける

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
		// meshFnのcolorSetのすべての頂点の色の配列をvertexColorListにコピー
		meshFn.getVertexColors(vertexColorList);
		// meshFnのメッシュ法線リストをnormalsListに配列をコピー
		meshFn.getNormals(normalsList);
		// meshFnのvertexColorListの長さ
		int LenVertexList = vertexColorList.length();
		MFnSingleIndexedComponent fnComponent;
		MFnSingleIndexedComponent fullComponent = fnComponent.create(MFn::kMeshVertComponent);
		fnComponent.setCompleteData(LenVertexList);
		// 整数データ型の配列
		MIntArray vertexIndexList;
		// setCompleteDataで選択した要素の総数からすべての要素インデックスをvertexIndexListにコピーする
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