#ifndef SLLS_EXT_INC
#define SLLS_EXT_INC

#include "SLLS.h"

namespace run::slls {

    class SLLS_Ext{
    private:
        SLLS slls;
    public:
        SLLS_Ext();
        ~SLLS_Ext();
        void addDNode(const char* str, int at = 0);
        void deleteDNode(const char* str);
        void updateDNode(const char* oldStr, const char* newStr);
        void print();
        void writeToFile(const char* filename, const char* mode);
        int getLen();
        char* getElem(int position);
    };

}
#endif