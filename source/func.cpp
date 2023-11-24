#define _CRT_SECURE_NO_WARNINGS
#include "head.h"

ProcessFactory processFactory;

//	��������Ƿ����Ҫ��, ����Ҫ�󷵻� "�����ַ���",���򷵻� "# ������Ϣ"
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

//	�����ĵ�
void help() {
	std::string name = YuLi;
	Yellow();
	std::cout << "----------------------------------------------------------------------------------------\n";
	std::cout << name << " --help                                     �鿴�����ĵ�\n";
	std::cout << name << " --ChUpper str1 str2 [...]                  ���ַ���ת��Ϊ��д\n";
	std::cout << name << " --ChLower str1 str2 [...]                  ���ַ���ת��ΪСд\n";
	std::cout << name << " --Radix   a-to-b  number                   ��a������ת��Ϊb����\n";
	std::cout << name << " --MCF(MaxCommonFactor) num1 num2           ��num1��num2���������\n";
	std::cout << name << " --MCM(MinCommonMultiples) num1 num2        ��num1��num2����С������\n";
	std::cout << name << " --Bezout a b                               ��Bezout��ʽsa+tb=(a,b)�Ľ�s��t\n";
	std::cout << name << " --Factor num                               �ֽ�����num\n";
	std::cout << name << " --Eular  num                               ��num��ŷ������ֵ\n";
	std::cout << name << " --Congruence_1  a b m                      ������ ax��b(mod m)��һ��ͬ��ʽ\n";
	std::cout << "----------------------------------------------------------------------------------------\n";
	White();
}

//	ת���ַ���Сд
std::string TransformCase(std::string str, int mode) {
	if (mode == 1)
		transform(str.begin(), str.end(), str.begin(), ::tolower);
	else if (mode == 2) {
		transform(str.begin(), str.end(), str.begin(), ::toupper);
	}
	return str;
}



