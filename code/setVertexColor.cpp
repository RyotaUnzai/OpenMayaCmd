/*
# coding : ryota unzai.
# Mana   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
選択した各オブジェクトに任意のvertex colorを付ける

setVertexColor;

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
#include <maya\MItSelectionList.h>
#include <maya\MArgDatabase.h>
#include <maya\MSyntax.h>
#include <maya\MDistance.h>
#include <maya\MObject.h>
#include <maya\MDagPath.h>
#include <maya\MIntArray.h>
#include <maya\MFloatVectorArray.h>
#include <maya\MFnSingleIndexedComponent.h>
#include <maya\MFn.h>
#include <maya\MArrayDataHandle.h>
#include <maya\MDataHandle.h>

class setVertexColor : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable() const {
		return true;
	}
	static void *creator() { return new setVertexColor; }
	static MSyntax newSyntax();
private:
	MDGModifier dgMod;

};

const char *redFlag = "-r", *redLongFlag = "-redChannel";
const char *greenFlag = "-g", *greenLongFlag = "-greenChannel";
const char *blueFlag = "-b", *blueLongFlag = "-blueChannel";
const char *alphaFlag = "-a", *alphaLongFlag = "-alphaChannel";

MSyntax setVertexColor::newSyntax() {
	MSyntax syntax;
	syntax.addFlag(redFlag, redLongFlag, MSyntax::kDouble);
	syntax.addFlag(greenFlag, greenLongFlag, MSyntax::kDouble);
	syntax.addFlag(blueFlag, blueLongFlag, MSyntax::kDouble);
	syntax.addFlag(alphaFlag, alphaLongFlag, MSyntax::kDouble);
	return syntax;
}

MStatus setVertexColor::doIt(const MArgList &args)
{
	double red = 0.0f;
	double green = 0.0f;
	double blue = 0.0f;
	double alpha = 1.0f;
	MArgDatabase argData(syntax(), args);
	if (argData.isFlagSet(redFlag)) {
		argData.getFlagArgument(redFlag, 0, red);
	}
	if (argData.isFlagSet(greenFlag)){
		argData.getFlagArgument(greenFlag, 0, green);
	}
	if (argData.isFlagSet(blueFlag)) {
		argData.getFlagArgument(blueFlag, 0, blue);
	}
	if (argData.isFlagSet(alphaFlag)) {
		argData.getFlagArgument(alphaFlag, 0, alpha);
	}

	MSelectionList selList;
	MGlobal::getActiveSelectionList(selList);
	MDagPath dagPath;
	MItSelectionList iter(selList, MFn::kMesh);
	for (; !iter.isDone(); iter.next()) {
		iter.getDagPath(dagPath);
		MFnMesh meshFn;
		meshFn.setObject(dagPath);
		MFnDependencyNode dnFn = dagPath.node();

		MColorArray vertexColorList;
		meshFn.getVertexColors(vertexColorList);

		int LenVertexList = vertexColorList.length();
		vertexColorList.setLength(LenVertexList);

		MIntArray vertexIndexList;
		for (signed int count = 0; count < LenVertexList; count++) {
			/*
			MColor::MColorTypeを指定することでdoubleを使用することができる
			vertexColorList.set(MColor(red, green, blue, alpha), count);
			上のようにMColorTypeを指定しない場合はfloatのみ受け付けるためstatic_cast<float>(doule)で
			doubleをfloatにcastしないとうまく動かない
			*/
			vertexColorList.set(MColor(MColor::kRGB, red, green, blue, alpha), count);
			vertexIndexList.append(count);
		}

		meshFn.setVertexColors(vertexColorList, vertexIndexList, 0, MFnMesh::MColorRepresentation::kRGB);
	}
	return redoIt();
}


MStatus setVertexColor::undoIt() {
	return MS::kSuccess;
}

MStatus setVertexColor::redoIt() {
	return MS::kSuccess;
}


MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "UnPySide", "1.0", "Any");

	MStatus stat;
	stat = plugin.registerCommand("setVertexColor", setVertexColor::creator, setVertexColor::newSyntax);
	if (!stat)
		stat.perror("registerCommand failed");
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	MStatus stat;
	stat = plugin.deregisterCommand("setVertexColor");

	if (!stat)
		stat.perror("deregisterCommand failed");
	return MS::kSuccess;
}