#include "head.h"

void Start(int argc, char* argv[]) {
	//	基本格式
	if (argc < 2) {
		Yellow("Please use \"yuli.exe --help\" to read help documentation\n");
		PauseExit(1);
	}
	//	获取命令
	std::string command = std::string(argv[1]);
	//	检查命令
	command = CheckCommand_(command);
	//	报错就结束
	if (command[0] == '#') {
		Red(command);
		PauseExit(1);
	}
	//	保存命令参数
	SaveCommandArgs(argv, 2, argc - 1);
	//	分析命令
	std::string analyse = AnalyseCommand(command);
	//	报错就打印输出然后结束
	if (analyse != "") {
		Red(analyse);
		PauseExit(1);
	}
	//	执行命令
	ExecuteCommand(command);
}

int main(int argc, char* argv[]) {

	//LoadConfigFile();		//	最后再开启

	HideCursor();
	 
	Start(argc, argv);		//	启动程序

	PauseExit(0);			//	退出程序
	
}
