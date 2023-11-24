#define _CRT_SECURE_NO_WARNINGS
#include "head.h"

void PauseExit(int status) {
	ShowCursor();
	std::string realpath = GetRealPathByWindows();

	if (realpath.substr(0, 5) != "\\\\wsl")
		system("pause");

	exit(status);
}

void Red()			//���ô�ӡ����Ϊ��ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
}

void White()		//���ô�ӡ����Ϊ��ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
}

void Green()		//���ô�ӡ����Ϊ��ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
}

void Blue()			//���õ�ǰ���Ϊ��ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
}

void Yellow() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
}

void Yellow(std::string str) {
	Yellow();
	std::cout << str;
	White();
}

void Red(std::string str) {
	Red();
	std::cout << str;
	White();
}

void Green(std::string str) {
	Green();
	std::cout << str;
	White();
}

void Blue(std::string str) {
	Blue();
	std::cout << str;
	White();
}


void HideCursor() {	//	���ع��
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void ShowCursor() {	//	��ʾ���
	CONSOLE_CURSOR_INFO cursor_info = { 1, 1 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void Cls() {
	system("cls");
}

std::vector<std::string> _args_;

void SaveCommandArgs(char* commandArgs[], int start, int end) {	//	���������в���
	for (int i = start; i <= end; ++i) {
		_args_.push_back(std::string(commandArgs[i]));
	}
}

std::string GetCommandArgs(int i) {		//	��ȡ�����в���
	if (i < 0 || _args_.size() <= i) {
		return "";
	}
	return _args_.at(i);
}

int GetCommandArgsNum() {		//	��ȡ�����в�����������
	return _args_.size();
}

std::string GetRealPathByWindows() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return std::string(buffer);
}
