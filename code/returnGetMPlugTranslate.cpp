/*
# coding : ryota unzai.
# web   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
MPlugを使ってノードの値を取る
MFnDependencyNodeのfindPlug関数を使っています。
まず対象のノードをMFnDependencyNodeクラスに設定してからfindPlug関数を実行します。
すると、そのノードのアトリビュートへのプラグが返されます。プラグにはノードとアトリビュートが記録されています。

オブジェクトを選択して実行
returnGetMPlugTranslate;

Result:
// arg1 //
// arg3 //
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

class returnGetMPlugTranslate : public MPxCommand
{
public:
	MStatus doIt(const MArgList& args);
	static void* creator();
};

MStatus returnGetMPlugTranslate::doIt(const MArgList& args)
{
	//選択リストを取得し、イテレーターを使用して選択リストをトレースする
	MSelectionList selList;
	MGlobal::getActiveSelectionList(selList);
	MItSelectionList listIt(selList);

	//MDagPathをからfullPathName関数でノード名を得ます。
	MDagPath path;
	listIt.getDagPath(path);
	MString pathName = path.fullPathName();

	// ------------------------------- start debug print 
	char pathNameChar[300];
	sprintf_s(pathNameChar, "pathName: %s", pathName.asChar());
	MGlobal::displayInfo(pathNameChar);
	// ------------------------------- end debug print 

	//getDependNode(0)で0番のMObjectを取り出す。MFnDependencyNodeにそのまま設定。
	MObject node;
	selList.getDependNode(0, node);
	MFnDependencyNode nodeFn(node);

	// findPlug関数にアトリビュート名を指定して、プラグを取得。
	//（二つ目の引数はとりあえずFalseに）
	MPlug translatePlug = nodeFn.findPlug("translate", false);

	/*
	translatePlug(MPlug)には、translateという
	tx, ty, tzの親となるアトリビュートが記録されています。
	tx, ty, tzの値を取り出すには、plugの子のプラグを得て、その値を出力します
	*/
	MPlug txPlug = translatePlug.child(0);
	MPlug tyPlug = translatePlug.child(1);
	MPlug tzPlug = translatePlug.child(2);


	// ------------------------------- start debug print 
	char txPlugChar[100];
	char tyPlugChar[100];
	char tzPlugChar[100];
	sprintf_s(txPlugChar, "txPlug: %lf", txPlug.asDouble());
	sprintf_s(tyPlugChar, "tyPlug: %lf", tyPlug.asDouble());
	sprintf_s(tzPlugChar, "tzPlug: %lf", tzPlug.asDouble());

	MGlobal::displayInfo(txPlugChar);
	MGlobal::displayInfo(tyPlugChar);
	MGlobal::displayInfo(tzPlugChar);
	// ------------------------------- end debug print 

	return MS::kSuccess;
}

void* returnGetMPlugTranslate::creator()
{
	return new returnGetMPlugTranslate;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("returnGetMPlugTranslate", returnGetMPlugTranslate::creator);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("returnGetMPlugTranslate");

	return MS::kSuccess;
}