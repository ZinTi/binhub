#include "slls_ext.h"
#include <cstdio>
#include "SLLS.h"

namespace run::slls {
	SLLS_Ext::SLLS_Ext() {
		this->slls = InitSLLS(); // 初始化
	}
	SLLS_Ext::~SLLS_Ext() {}
	void SLLS_Ext::addDNode(const char* str, int at) {
		if (at) {
			InsertNodeSLLSAtHead(this->slls, str);
		}
		else { // 默认从尾部插入
			InsertNodeSLLSAtTail(this->slls, str);
		}
	}
	void SLLS_Ext::deleteDNode(const char* str) {
		DeleteNodeSLLSByContent(this->slls, str);
	}
	void SLLS_Ext::updateDNode(const char* oldStr, const char* newStr) {
		ModifyNodeSLLSByContent(this->slls, oldStr, newStr);
	}
	void SLLS_Ext::print() { // 遍历输出slls
		int len_slls = GetSLLSLength(this->slls);
		for (int i = 0; i < len_slls; i++) {
			printf("%s\n", FindNodeSLLSByPosition(this->slls, i));
		}
	}
	void SLLS_Ext::writeToFile(const char* filename, const char* mode) { // 写入文件
		int len_slls = GetSLLSLength(this->slls);
		FILE* fp = fopen(filename, mode);
		for (int i = 0; i < len_slls; i++) {
			fprintf(fp, "%s\n", FindNodeSLLSByPosition(this->slls, i));
		}
		fclose(fp);
	}
	int SLLS_Ext::getLen() {
		return GetSLLSLength(this->slls);
	}
	char* SLLS_Ext::getElem(int position) {
		return FindNodeSLLSByPosition(this->slls, position);
	}
}

/*
///////////////////////////// 内存中函数的类 ////////////////////////////////////////
class SRC_Func{
	private:
		char* returnType; // 函数返回类型
		char* funcName; // 函数名
		SLLS_Ext paraList; // 函数形参列表
		SLLS_Ext funcBody; // 函数体

	public:
		SRC_Func(){
			returnType = NULL;
			funcName = NULL;
		}
		~SRC_Func(){}
		int setReturnType(const char* newReturnType){
			if(this->returnType == NULL){
				this->returnType = (char*)malloc(strlen(newReturnType)+1);
			}else if(returnType!=NULL && sizeof(this->returnType) < strlen(newReturnType)+1){
				free(this->returnType);
				this->returnType = (char*)malloc(strlen(newReturnType)+1);
			}
			strcpy(this->returnType, newReturnType);
			return 0;
		}
		int setFuncName(const char* newFuncName){
			if(this->funcName == NULL){
				this->funcName = (char*)malloc(strlen(newFuncName)+1);
			}else if(this->funcName!=NULL && sizeof(this->funcName) < strlen(newFuncName)+1){
				free(this->funcName);
				this->funcName = (char*)malloc(strlen(newFuncName)+1);
			}
			strcpy(this->funcName, newFuncName);
			return 0;
		}
		void setParaList(int argC, char* argV[]){
			for(int i=0;i<argC;i++){
				this->paraList.addDNode(argV[i]);
			}
		}
		void setFuncBody(int statC, char* statV[]){
			for(int i=0;i<statC;i++){
				this->funcBody.addDNode(statV[i]);
			}
		}
		void printFunc(){
			int len_pl = paraList.getLen();
			int len_fb = funcBody.getLen();
			ColorPrintf(0x02, "%s ", returnType);
			ColorPrintf(0x07, "%s(", funcName);
			for(int i=0 ; i<len_pl ; i++){
				if(i = len_pl-1){
					printf("%s)", paraList.getElem(i));
				}else{
					printf("%s, ", paraList.getElem(i));
				}
			}
			printf("{");
			for(int i=0 ; i<len_fb ; i++){
				printf("\t%s\n", paraList.getElem(i));
			}
			printf("}");
		}
};

///////////////////////////// 内存中程序源文件的类 ///////////////////////////////////
class SRC_PROG{
	private:
		SLLS_Ext incL;      // #include 预处理命令链表
		SLLS_Ext defL;    // #define 预处理命令链表
		SRC_Func main;      // main()
		SRC_Func func;      // a func();

	public:
		SRC_PROG(){}
		~SRC_PROG(){}

}; */