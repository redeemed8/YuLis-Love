#include "head.h"

#define ConfigPath "conf/system.ini"

//	�ɶ�ȡ����"ok",���򷵻ؿ��ַ��� ""
std::string CheckConfigByRow(std::string line) {
	if (line.size() < 5)	return "";
	std::regex pattern("^([a-zA-Z0-9_]+) = ([a-zA-Z0-9_]+)$");
	return std::regex_match(line, pattern) ? "ok" : "";
}

//	��ȡ�����ļ�·��
std::string GetConfigPath() {
	std::string path = GetRealPathByWindows();
	int count = 0;
	for (int i = path.size() - 1; i >= 0; --i) {
		if (path[i] != '\\')	++count;
		else break;
	}
	path = path.substr(0, path.size() - count);
	for (int i = 0; i < path.size(); ++i)
		if (path[i] == '\\')	path[i] = '/';
	return path + ConfigPath;
}

//	���������ļ����ɹ����� ���ַ���"", ʧ�ܷ��� ֱ�ӱ����˳�
bool LoadConfigFile() {
	//	��ȡ�����ļ�·��
	std::string configfilepath = GetConfigPath();

	//	�����ļ�Ŀ¼�̶�Ϊ ��ǰĿ¼�� conf/system.ini
	std::ifstream file(configfilepath);
	if (!file.is_open()) {
		Red("# Not found config file. It should be /conf/system.ini in the current directory!\n");
		PauseExit(1);
	}
	std::string line;
	while (std::getline(file, line)) {
		if (CheckConfigByRow(line) != "") {
			//	�ɼ���
			std::string k, v;
			int len = line.size();
			for (int i = 0; i < len; ++i) {
				if (line[i] == ' ')	break;
				k += line[i];
			}
			for (int i = len - 1; i >= 0; --i) {
				if (line[i] == ' ')	break;
				v = line[i] + v;
			}
			if (k == "sys") {
				Global_Config.Sys = v;
			}
		}
	}
	file.close();
	return true;
}



