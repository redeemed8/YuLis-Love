#include "head.h"

void Start(int argc, char* argv[]) {
	//	������ʽ
	if (argc < 2) {
		Yellow("Please use \"yuli.exe --help\" to read help documentation\n");
		PauseExit(1);
	}
	//	��ȡ����
	std::string command = std::string(argv[1]);
	//	�������
	command = CheckCommand_(command);
	//	����ͽ���
	if (command[0] == '#') {
		Red(command);
		PauseExit(1);
	}
	//	�����������
	SaveCommandArgs(argv, 2, argc - 1);
	//	��������
	std::string analyse = AnalyseCommand(command);
	//	����ʹ�ӡ���Ȼ�����
	if (analyse != "") {
		Red(analyse);
		PauseExit(1);
	}
	//	ִ������
	ExecuteCommand(command);
}

int main(int argc, char* argv[]) {

	//LoadConfigFile();		//	����ٿ���

	HideCursor();
	 
	Start(argc, argv);		//	��������

	PauseExit(0);			//	�˳�����
	
}
