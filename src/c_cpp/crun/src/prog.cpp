#include "prog.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "exception.h"
#include "safe_exit.h"
#include "str.h"
#include "use_template.h"
namespace run::prog{
    IcoToWinRes::IcoToWinRes() : icoFn(nullptr), rcFn(nullptr), resFn(nullptr){
    }
    IcoToWinRes::~IcoToWinRes(){
        delete icoFn;
        delete rcFn;
        delete resFn;
    }
    void IcoToWinRes::setIcoFn(const char* newIcoFn){
        if (newIcoFn == nullptr){
            PrintError("IcoToWinRes.setIcoFn()", "The newIcoFn parameter is NULL");
            SafeExit(0);
        }
        icoFn = new char[strlen(newIcoFn) + 1];
        if (icoFn == nullptr){
            PrintError("IcoToWinRes.setIcoFn()", "Memory allocation failed!");
            SafeExit(-1);
        }
        strcpy(icoFn, newIcoFn);
    }
    char* IcoToWinRes::ConvIcoToRes(){
        size_t len_srcFn = strlen(icoFn);
        rcFn = new char[len_srcFn + 4];  // Allocate memory for ".rc"
        resFn = new char[len_srcFn + 5]; // Allocate memory for ".res"
        if (rcFn == nullptr || resFn == nullptr){
            PrintError("IcoToWinRes.ConvIcoToRes()", "Memory allocation failed!");
            SafeExit(-1);
        }
        snprintf(rcFn, len_srcFn + 4, "%s.rc", icoFn);
        snprintf(resFn, len_srcFn + 5, "%s.res", icoFn);
        // Write .rc file
        FILE* fp = fopen(rcFn, "w");
        if (fp == nullptr){
            PrintError("IcoToWinRes.ConvIcoToRes()", "Failed to open .rc file");
            SafeExit(-1);
        }
        fprintf(fp, "A ICON \"%s\"", icoFn);
        fclose(fp);
        // Convert to .res
        char* cmdStr = new char[2 * len_srcFn + 50];
        if (cmdStr == nullptr){
            PrintError("IcoToWinRes.ConvIcoToRes()", "Memory allocation failed!");
            SafeExit(-1);
        }
        sprintf(cmdStr, "windres -i %s --input-format=rc -o %s -O coff", rcFn, resFn);
        system(cmdStr);
        delete cmdStr;
        // Clean up
        remove(rcFn);
        delete rcFn;
        delete icoFn;
        icoFn = nullptr;
        return resFn;
    }
    MemoryProg::MemoryProg() {}
    MemoryProg::~MemoryProg() {}
    void MemoryProg::addInc(const char* incStr){
        incList.addDNode(incStr);
    }
    void MemoryProg::addDef(const char* defStr){
        defList.addDNode(defStr);
    }
    void MemoryProg::addMainStatement(const char* statement){
        mainbody.addDNode(statement);
    }
    void MemoryProg::addFunc(const char* func){
        funcList.addDNode(func);
    }
    void MemoryProg::addStructUnion(const char* s_u){
        struct_union.addDNode(s_u);
    }
    void MemoryProg::rmInc(const char* incStr){
        incList.deleteDNode(incStr);
    }
    void MemoryProg::rmDef(const char* defStr){
        defList.deleteDNode(defStr);
    }
    void MemoryProg::rmMainStatement(const char* statement){
        mainbody.deleteDNode(statement);
    }
    void MemoryProg::rmFunc(const char* func){
        funcList.deleteDNode(func);
    }
    void MemoryProg::rmStructUnion(const char* s_u){
        struct_union.deleteDNode(s_u);
    }
    void MemoryProg::writeToFile(const char* filename, const char* mode){
        int len_incList = incList.getLen();
        int len_defList = defList.getLen();
        int num_s_u = struct_union.getLen();
        int num_func = funcList.getLen();
        int num_statements = mainbody.getLen();
        FILE* fp = fopen(filename, mode);
        for (int i = 0; i < len_incList; i++){
            fprintf(fp, "%s\n", incList.getElem(i));
        }
        for (int i = 0; i < len_defList; i++){
            fprintf(fp, "%s\n", defList.getElem(i));
        }
        for (int i = 0; i < num_s_u; i++){
            fprintf(fp, "%s\n", struct_union.getElem(i));
        }
        for (int i = 0; i < num_func; i++){
            fprintf(fp, "%s\n", funcList.getElem(i));
        }
        fprintf(fp, "int main(int argc, char* argv[]){\n");
        for (int i = 0; i < num_statements; i++){
            fprintf(fp, "\t%s\n", mainbody.getElem(i));
        }
        fprintf(fp, "\treturn 0;\n}\n");
        fclose(fp);
    }
    SrcToELF::SrcToELF() : srcFn(nullptr), objFn(nullptr), resFn(nullptr), ELF(nullptr){
    }
    SrcToELF::~SrcToELF(){
        delete srcFn;
        delete objFn;
        delete resFn;
        delete ELF;
    }
    void SrcToELF::setSrcFn(const char* nSrcFn){
        if (nSrcFn == nullptr){
            PrintError("SrcToELF.setSrcFn()", "Error! The arguments cannot be NULL!");
            SafeExit(-1);
        }
        srcFn = new char[strlen(nSrcFn) + 1];
        if (srcFn == nullptr){
            PrintError("SrcToELF.setSrcFn()", "Memory allocation failed!");
            SafeExit(-1);
        }
        strcpy(srcFn, nSrcFn);
        char tempFn[1024];
        ChFilenameExtension(nSrcFn, tempFn, ".o");
        setObjFn(tempFn);
        ChFilenameExtension(nSrcFn, tempFn, ".exe");
        setELF(tempFn);
    }
    void SrcToELF::setObjFn(const char* nObjFn){
        if (nObjFn == nullptr){
            PrintError("SrcToELF.setObjFn()", "Error! The arguments cannot be NULL!");
            SafeExit(-1);
        }
        objFn = new char[strlen(nObjFn) + 1];
        if (objFn == nullptr){
            PrintError("SrcToELF.setObjFn()", "Memory allocation failed!");
            SafeExit(-1);
        }
        strcpy(objFn, nObjFn);
    }
    void SrcToELF::setELF(const char* nELF){
        if (nELF == nullptr){
            PrintError("SrcToELF.setELF()", "Error! The arguments cannot be NULL!");
            SafeExit(-1);
        }
        ELF = new char[strlen(nELF) + 1];
        if (ELF == nullptr){
            PrintError("SrcToELF.setELF()", "Memory allocation failed!");
            SafeExit(-1);
        }
        strcpy(ELF, nELF);
    }
    int SrcToELF::inputELF(void){
        printf("Executable file name > ");
        fgets(ELF, 1024, stdin);
        return 0;
    }
    char* SrcToELF::getELF(void){
        return ELF;
    }
    int SrcToELF::CompileAssemble(void){
        FILE* fp = fopen(srcFn, "r");
        if (fp){
            fclose(fp);
        }
        else{
            printf("Files not found, using template name as %s...\n", srcFn);
            UseTemplate(srcFn);
        }
        char* CACmd = new char[PATH_MAX];
        sprintf(CACmd, "gcc -c -o %s %s", objFn, srcFn);

        system(CACmd);
        delete CACmd;
        return 0;
    }
    char* SrcToELF::Link(){
        char* LinkCmd = new char[PATH_MAX];
        if (resFn != nullptr){
            sprintf(LinkCmd, "g++ -Os %s %s -o %s -s", objFn, resFn, ELF);
        }
        else{
            sprintf(LinkCmd, "g++ -Os %s -o %s -s", objFn, ELF);
        }
        system(LinkCmd);
        delete LinkCmd;
        remove(resFn);
        remove(objFn);
        return ELF;
    }
}