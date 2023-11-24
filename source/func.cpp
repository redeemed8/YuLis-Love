#define _CRT_SECURE_NO_WARNINGS
#include "head.h"

ProcessFactory processFactory;

//	检查命令是否符合要求, 符合要求返回 "命令字符串",否则返回 "# 错误信息"
std::string CheckCommand_(std::string& command) {
	int len = command.size();
	if (len <= 2 || len >= 20 || command[0] != '-' || command[1] != '-')
		return UNKNOWN_COMMAND;
	int i = 0;
	std::string ret = "";
	for (int i = 2; i < command.size(); ++i) {
		ret += command[i];
	}
	return ret;
}

#define YuLi "yuli.exe"
#define Tool "[tools]/[Tools.exe]"

//	帮助文档
void help() {
	std::string name = YuLi;
	Yellow();
	std::cout << "----------------------------------------------------------------------------------------\n";
	std::cout << name << " --help                                     查看帮助文档\n";
	std::cout << name << " --ChUpper str1 str2 [...]                  将字符串转换为大写\n";
	std::cout << name << " --ChLower str1 str2 [...]                  将字符串转换为小写\n";
	std::cout << name << " --Radix   a-to-b  number                   将a进制数转换为b进制\n";
	std::cout << name << " --MCF(MaxCommonFactor) num1 num2           求num1和num2的最大公因数\n";
	std::cout << name << " --MCM(MinCommonMultiples) num1 num2        求num1和num2的最小公倍数\n";
	std::cout << name << " --Bezout a b                               求Bezout等式sa+tb=(a,b)的解s和t\n";
	std::cout << name << " --Factor num                               分解因数num\n";
	std::cout << name << " --Eular  num                               求num的欧拉函数值\n";
	std::cout << name << " --Congruence_1  a b m                      解形如 ax≡b(mod m)的一次同余式\n";
	std::cout << "----------------------------------------------------------------------------------------\n";
	White();
}

//	转换字符大小写
std::string TransformCase(std::string str, int mode) {
	if (mode == 1)
		transform(str.begin(), str.end(), str.begin(), ::tolower);
	else if (mode == 2) {
		transform(str.begin(), str.end(), str.begin(), ::toupper);
	}
	return str;
}



