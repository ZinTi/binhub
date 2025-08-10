#include "handle_cmd_winres.h"
#include <iostream>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif
#include "win_res.h"


namespace cpro{

int handle_cmd_winres(int argc, char* argv[]){
    if(argc < 2){
        binhub_cerr("Usage: cpro %s <input> [output]", argv[0]); putchar('\n');
        return 1;
    }

    cpro::WinRes win_res(argv[1]);
    if(argc == 3){
        win_res.set_res_fn(argv[2]);
    }
    return win_res.generate_res_file();    // 生成 .res 文件
}


};
