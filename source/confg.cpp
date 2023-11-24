#include "head.h"

#define ConfigPath "conf/system.ini"

//	可读取返回"ok",否则返回空字符串 ""
std::string CheckConfigByRow(std::string line) {
	if (line.size() < 5)	return "";
	std::regex pattern("^([a-zA-Z0-9_]+) = ([a-zA-Z0-9_]+)$");
	return std::regex_match(line, pattern) ? "ok" : "";
}

//	获取配置文件路径
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

//	加载配置文件，成功返回 空字符串"", 失败返回 直接报错退出
bool LoadConfigFile() {
	//	获取配置文件路径
	std::string configfilepath = GetConfigPath();

	//	配置文件目录固定为 当前目录下 conf/system.ini
	std::ifstream file(configfilepath);
	if (!file.is_open()) {
		Red("# Not found config file. It should be /conf/system.ini in the current directory!\n");
		PauseExit(1);
	}
	std::string line;
	while (std::getline(file, line)) {
		if (CheckConfigByRow(line) != "") {
			//	可加载
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



