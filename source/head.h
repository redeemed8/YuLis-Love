#pragma once
#include <iostream>
#include <windows.h>
#include <map>
#include <vector>
#include <any>
#include <thread>
#include <algorithm>
#include <regex>
#include <string>
#include <fstream>
#include <set>

#define PROCESSLEN 110
#define PROCESS_DELAY 1
#define PROCESS_OUTPUT 1
#define PROCESS_NOPUT 0
//	����
class Process {
private:
	double Start;						//	��ʼֵ
	double End;							//	����ֵ
	double CurValue;					//	��ǰֵ
	std::vector<std::any>& Result;		//	�������
	bool Finish;						//	������־
	int Mode;							//	ģʽ

public:
	Process();
	Process(double start, double end);
	void Delay();						//	�ӳ�
	double CheckCurProcess();			//	�鿴����
	double GetEnd();					//	��ȡend
	bool SetEnd(double _end);			//	������ֵֹ
	bool SetMode(int _mode);			//	����ģʽ
	bool SetFinish(bool isFinish);		//	�������״̬
	bool Finished();					//	�Ƿ���ֹ
	bool AddResults(std::any& a);		//	��ӽ��
	bool UpdateProcess();				//	���½���
};

class ProcessFactory {
public:
	Process* Create_Process(double start, double end);
};

class Config {
public:
	std::string Sys;
public:
	Config();

};

static Config Global_Config;

#define UNKNOWN_COMMAND "# Unknown Command! Please use --help to read help documentation!\n"
#define ARGS_NOENOUGH "# The number of required parameters is insufficient!\nPlease use --help to read help documentation!\n"

//	--------- Source ---------
std::string ToString(double num);			//	�� 0 ~ 999ת��Ϊ�ַ���
void ParseAnyAndPrint(std::any a);			//	��any����ת��Ϊ��Ӧ�����ͺ��ӡ���
void RunUpdateProcess(Process* p);			//	����ִ�н��ȸ���
std::string AnalyseCommand(std::string command);	//	��������
void ExecuteCommand(std::string command);			//	ִ������


//	--------- func ---------
void help();												//	�����ĵ�
std::string TransformCase(std::string str, int mode);		//	ת���ַ���Сд
std::string CheckCommand_(std::string& command);			//	��������Ƿ����Ҫ��


//	--------- console ---------
void PauseExit(int status);					//	��ͣ���˳�
void Red();							//	�޸Ĵ�ӡ����Ϊ��ɫ
void White();						//	�޸Ĵ�ӡ����Ϊ��ɫ
void Green();						//	�޸Ĵ�ӡ����Ϊ��ɫ
void Blue();						//	�޸Ĵ�ӡ����Ϊ��ɫ
void Yellow();						//	�޸Ĵ�ӡ����Ϊ��ɫ
void Yellow(std::string str);
void Red(std::string str);
void Green(std::string str);
void Blue(std::string str);
void HideCursor();					//	���ع��
void ShowCursor();					//	��ʾ���
void Cls();							//	����
void SaveCommandArgs(char* commandArgs[], int start, int end);				//	���������в���
std::string GetCommandArgs(int i);	//	��ȡ�����в���
int GetCommandArgsNum();			//	��ȡ�����в�����������
std::string GetRealPathByWindows();	//	��ȡwindows��ǰ·��


//	--------- mathbase ---------
std::string division(const std::string& dividend, int divisor);					//	�߾��ȳ���
std::string CheckRadixStr(std::string radix, std::string radixNum);				//	�����Ʋ���
std::string RadixToDecimal(std::string radix, std::string radixNum);			//	�������ת10����
std::string DecimalToRadix(std::string decimal, std::string toRadixNum);		//	10����ת�������
std::string Radix1ToRedix2(std::string howToTrans, std::string radix);			//	�������ת��
std::string MaxCommonFactor(std::string& num1, std::string& num2);				//	�������
std::string MinCommonMultiples(std::string& num1, std::string& num2);			//	��С������
std::string CheckStrIsNumber(std::string& str);									//	����ַ����ǲ�������
bool IsGreatIntMax(std::string str);											//	�ж����ִ��Ƿ���� int_max
bool IsGreatLongLongMax(std::string str);										//	�ж����ִ��Ƿ���� long_long_max
void ExtendedEuclidean(int a, int b, int& s, int& t);							//	��ⱴ���ʽ sa+tb=(a,b)
int Get_i_();																	//	��ȡ���Ȳ���i,�������
std::string factorize(long long num);										    //	�ֽ�����
std::string Eular(std::string num);												//	ŷ������
std::string Congruence_1(int a, int b, int m);									//	���һ��ͬ��ʽ,���� ax �� b (mod m)
std::string add(std::string num1, std::string num2);							//	�߾���
std::string subtraction(std::string num1, std::string num2);					//	�߾���
std::string multiply(std::string num1, std::string num2);						//	�߾���

//	-------------- mathsenior -----------------
std::tuple<std::string, std::string> division2(std::string dividend, std::string divisor);	//	���׸߾���
std::tuple<std::string, std::string> division3(std::string dividend, std::string divisor);	//	���������߾���

//	--------- config ---------
bool LoadConfigFile();			//	���������ļ���Ϣ���ڴ�
