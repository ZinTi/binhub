#ifndef SRCTOBIN_INC
#define SRCTOBIN_INC
#include "slls_ext.h"

namespace run::prog {
	typedef struct WinRes {
		char* icoFn;
		char* rcFn;
		char* resFn;
	} WinRes, * pWinRes;

	class IcoToWinRes {
	private:
		char* icoFn;
		char* rcFn;
		char* resFn;

	public:
		IcoToWinRes();
		~IcoToWinRes();
		void setIcoFn(const char* newIcoFn);
		char* ConvIcoToRes();
	};

	class MemoryProg {
	private:
		run::slls::SLLS_Ext incList;
		run::slls::SLLS_Ext defList;
		run::slls::SLLS_Ext funcList;
		run::slls::SLLS_Ext struct_union;
		run::slls::SLLS_Ext mainbody;

	public:
		MemoryProg();
		~MemoryProg();
		void addInc(const char* incStr);
		void addDef(const char* defStr);
		void addMainStatement(const char* statement);
		void addFunc(const char* func);
		void addStructUnion(const char* s_u);

		void rmInc(const char* incStr);
		void rmDef(const char* defStr);
		void rmMainStatement(const char* statement);
		void rmFunc(const char* func);
		void rmStructUnion(const char* s_u);

		void writeToFile(const char* filename = "~temp.c", const char* mode = "w");
	}; // class: run::prog::MemoryProg

	class SrcToELF {
	private:
		char* srcFn;
		char* objFn;
		char* resFn;
		char* ELF;

	public:
		SrcToELF();
		~SrcToELF();
		void setSrcFn(const char* nSrcFn);
		void setObjFn(const char* nObjFn);
		void setELF(const char* nELF);
		int inputELF(void);
		char* getELF(void);
		int CompileAssemble(void);
		char* Link();
	}; // class: run::prog::SrcToELF
}

#endif
