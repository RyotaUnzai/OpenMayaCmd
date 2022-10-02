/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
MDGContextを使ってMPlugに計算して欲しい時間の値を取得する

getTransleteFrameTime;

Result:
// Translate: 2.500000, 2.500000, 2.500000 //
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
#include <maya\MTime.h>
#include <maya\MSyntax.h>
#include <maya\MArgDatabase.h>
#include <maya\MDGModifier.h>
#include <maya\MDoubleArray.h>

class getTransleteFrameTime : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);

	static void *creator() { return new getTransleteFrameTime; }
	static MSyntax newSyntax();

private:
	MDGModifier dgMod;
};

const char *frameTimeFlag = "-t", *frameTimeLongFlag = "-time";
const char *unitFlag = "-u", *unitLongFlag = "-unit";

MSyntax getTransleteFrameTime::newSyntax() {
	MSyntax syntax;
	syntax.addFlag(frameTimeFlag, frameTimeLongFlag, MSyntax::kDouble);
	syntax.addFlag(unitFlag, unitLongFlag, MSyntax::kString);
	return syntax;
}


MStatus getTransleteFrameTime::doIt(const MArgList& args)
{

	double time = 5.0f;
	MArgDatabase argData(syntax(), args);
	if (argData.isFlagSet(frameTimeFlag))
		argData.getFlagArgument(frameTimeFlag, 0, time);

	MSelectionList sList;
	MGlobal::getActiveSelectionList(sList);
	MObject obj;
	sList.getDependNode(0, obj);

	MFnDependencyNode dgFn(obj);

	MPlug plugTx = dgFn.findPlug("tx");
	MPlug plugTy = dgFn.findPlug("ty");
	MPlug plugTz = dgFn.findPlug("tz");

	MString unit;
	if (argData.isFlagSet(unitFlag))
		argData.getFlagArgument(unitFlag, 0, unit);

	// 適当な初期値で初期化
	MTime::Unit mtime;
	if (unit == "kFilm") {
		mtime = MTime::kFilm;
	}
	else if (unit == "kNTSCFrame") {
		mtime = MTime::kNTSCFrame;
	}
	else if (unit == "kNTSCField") {
		mtime = MTime::kNTSCField;
	};
	MDGContext ctxTx(MTime(time, mtime));
	MDGContext ctxTy(MTime(time, mtime));
	MDGContext ctxTz(MTime(time, mtime));

	char ctxChar[300];
	// MPlugに計算して欲しい時間としてコンテキストを指定
	sprintf_s(ctxChar, "Translate: %lf, %lf, %lf", plugTx.asDouble(ctxTx), plugTy.asDouble(ctxTy), plugTz.asDouble(ctxTz));
	MGlobal::displayInfo(ctxChar);
	MDoubleArray values(3);
	values[0] = plugTx.asDouble(ctxTx);
	values[1] = plugTx.asDouble(ctxTy);
	values[3] = plugTx.asDouble(ctxTz);
	clearResult();
	setResult(values);
	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");
	plugin.registerCommand("getTransleteFrameTime", getTransleteFrameTime::creator, getTransleteFrameTime::newSyntax);
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("getTransleteFrameTime");

	return MS::kSuccess;
}