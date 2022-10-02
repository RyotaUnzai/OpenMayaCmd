/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
MFnMeshを使用してNumPolygonsを取得する

オブジェクトを選択して実行
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

	//選択リストを取得し、イテレーターを使用して選択リストをトレースする
	MSelectionList selList;
	MGlobal::getActiveSelectionList(selList);
	MDagPath dagPath;

	/*
	選択しているTransformノードの下にあるMesh Shapeのpathを取得し
	extendToShape()でdagPathの中身をmeshのpathへ変更する
	*/
	selList.getDagPath(0, dagPath);
	dagPath.extendToShape();

	//プラグを取り出す専用のMFnDependencyNodeを作成
	MFnDependencyNode dnFn = dagPath.node();
	// findPlug関数でメッシュデータのアトリビュート名を指定する
	MPlug plug = dnFn.findPlug("outMesh", false);

	/*
	プラグの値をMObjectとして取得
	MFnMeshに渡して、MFnMesh経由でメッシュデータを取り出す。
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