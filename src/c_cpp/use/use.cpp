#include <iostream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
	#include <windows.h>
#endif

#define HOME_PATH_VAR				"BINHUB_HOME"

#ifdef _WIN32
	#define USE_EXECUTABLE_PATH		"\\bin\\winnt\\subbin\\"
#else
	#define USE_EXECUTABLE_PATH		"/bin/linux/subbin/"
#endif

int main(int argc,char* argv[]){
	const char* home_path = getenv(HOME_PATH_VAR);
	if(!home_path){ // "!home_path" <=> "home_path == (void*)0"
		const std::string info = "The system environment variable `" HOME_PATH_VAR "` is not set.";
		std::cout << info << std::endl;
		return -1;
	}
	std::string full_comand_and_arg = home_path;		// 深复制，而不是复制 const char* 指针
	full_comand_and_arg.append(USE_EXECUTABLE_PATH);	// 拼接完整目标路径

	if(argc >= 2){
		if(argv[1] == "-l"){
			#ifdef _WIN32
				std::string cmd_tree_dir = "tree /f " + full_comand_and_arg;
			#else
				std::string cmd_tree_dir = "tree " + full_comand_and_arg;
			#endif
			system(cmd_tree_dir.c_str());
			return 1;
		}
		
		for(int i = 1 ; i < argc ; i++){
			if( i != 1 ){	// 不是第一个参数就一直追加后续参数（参数或选项）
				full_comand_and_arg.append(" \x22");	// x22 即 '"'
				full_comand_and_arg += argv[i];
				full_comand_and_arg.push_back('\x22');
			}else{
				full_comand_and_arg += argv[i];	// 第一个参数直接追加（程序完成路径名）
			}
		}
		system(full_comand_and_arg.c_str());		// 运行命令（原样传递参数）
		
	}else{
		std::cerr << "Insufficient parameters!" << std::endl;
		std::cout << "[USAGE]\n";
		std::cout << argv[0] << " <executable> [options...]    Run the executable in the directory `" << full_comand_and_arg << "`\n";
		std::cout << argv[0] << " <-l>                         List the files and subdirectories under the directory `" << full_comand_and_arg << "`\n";
	}
	return 0;
}