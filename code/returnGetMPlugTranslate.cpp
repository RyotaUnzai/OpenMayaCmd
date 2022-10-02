/*
# coding : ryota unzai.
# web   : https://unpyside.wixsite.com/unpyside
# e-mail : satellitear@gmail.com
MPlug���g���ăm�[�h�̒l�����
MFnDependencyNode��findPlug�֐����g���Ă��܂��B
�܂��Ώۂ̃m�[�h��MFnDependencyNode�N���X�ɐݒ肵�Ă���findPlug�֐������s���܂��B
����ƁA���̃m�[�h�̃A�g���r���[�g�ւ̃v���O���Ԃ���܂��B�v���O�ɂ̓m�[�h�ƃA�g���r���[�g���L�^����Ă��܂��B

�I�u�W�F�N�g��I�����Ď��s
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
	//�I�����X�g���擾���A�C�e���[�^�[���g�p���đI�����X�g���g���[�X����
	MSelectionList selList;
	MGlobal::getActiveSelectionList(selList);
	MItSelectionList listIt(selList);

	//MDagPath������fullPathName�֐��Ńm�[�h���𓾂܂��B
	MDagPath path;
	listIt.getDagPath(path);
	MString pathName = path.fullPathName();

	// ------------------------------- start debug print 
	char pathNameChar[300];
	sprintf_s(pathNameChar, "pathName: %s", pathName.asChar());
	MGlobal::displayInfo(pathNameChar);
	// ------------------------------- end debug print 

	//getDependNode(0)��0�Ԃ�MObject�����o���BMFnDependencyNode�ɂ��̂܂ܐݒ�B
	MObject node;
	selList.getDependNode(0, node);
	MFnDependencyNode nodeFn(node);

	// findPlug�֐��ɃA�g���r���[�g�����w�肵�āA�v���O���擾�B
	//�i��ڂ̈����͂Ƃ肠����False�Ɂj
	MPlug translatePlug = nodeFn.findPlug("translate", false);

	/*
	translatePlug(MPlug)�ɂ́Atranslate�Ƃ���
	tx, ty, tz�̐e�ƂȂ�A�g���r���[�g���L�^����Ă��܂��B
	tx, ty, tz�̒l�����o���ɂ́Aplug�̎q�̃v���O�𓾂āA���̒l���o�͂��܂�
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