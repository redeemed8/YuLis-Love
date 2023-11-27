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
//	进度
class Process {
private:
	double Start;						//	初始值
	double End;							//	结束值
	double CurValue;					//	当前值
	std::vector<std::any>& Result;		//	结果容器
	bool Finish;						//	结束标志
	int Mode;							//	模式

public:
	Process();
	Process(double start, double end);
	void Delay();						//	延迟
	double CheckCurProcess();			//	查看进度
	double GetEnd();					//	获取end
	bool SetEnd(double _end);			//	设置中止值
	bool SetMode(int _mode);			//	设置模式
	bool SetFinish(bool isFinish);		//	设置完成状态
	bool Finished();					//	是否终止
	bool AddResults(std::any& a);		//	添加结果
	bool UpdateProcess();				//	更新进度
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
std::string ToString(double num);			//	将 0 ~ 999转换为字符串
void ParseAnyAndPrint(std::any a);			//	将any对象转换为对应的类型后打印输出
void RunUpdateProcess(Process* p);			//	并发执行进度更新
std::string AnalyseCommand(std::string command);	//	分析命令
void ExecuteCommand(std::string command);			//	执行命令


//	--------- func ---------
void help();												//	帮助文档
std::string TransformCase(std::string str, int mode);		//	转换字符大小写
std::string CheckCommand_(std::string& command);			//	检查命令是否符合要求


//	--------- console ---------
void PauseExit(int status);					//	暂停并退出
void Red();							//	修改打印字体为红色
void White();						//	修改打印字体为白色
void Green();						//	修改打印字体为绿色
void Blue();						//	修改打印字体为蓝色
void Yellow();						//	修改打印字体为黄色
void Yellow(std::string str);
void Red(std::string str);
void Green(std::string str);
void Blue(std::string str);
void HideCursor();					//	隐藏光标
void ShowCursor();					//	显示光标
void Cls();							//	清屏
void SaveCommandArgs(char* commandArgs[], int start, int end);				//	保存命令行参数
std::string GetCommandArgs(int i);	//	获取命令行参数
int GetCommandArgsNum();			//	获取命令行参数的总数量
std::string GetRealPathByWindows();	//	获取windows当前路径


//	--------- mathbase ---------
std::string division(const std::string& dividend, int divisor);					//	高精度除法
std::string CheckRadixStr(std::string radix, std::string radixNum);				//	检查进制参数
std::string RadixToDecimal(std::string radix, std::string radixNum);			//	任意进制转10进制
std::string DecimalToRadix(std::string decimal, std::string toRadixNum);		//	10进制转任意进制
std::string Radix1ToRedix2(std::string howToTrans, std::string radix);			//	任意进制转换
std::string MaxCommonFactor(std::string& num1, std::string& num2);				//	最大公因数
std::string MinCommonMultiples(std::string& num1, std::string& num2);			//	最小公倍数
std::string CheckStrIsNumber(std::string& str);									//	检查字符串是不是数字
bool IsGreatIntMax(std::string str);											//	判断数字串是否大于 int_max
bool IsGreatLongLongMax(std::string str);										//	判断数字串是否大于 long_long_max
void ExtendedEuclidean(int a, int b, int& s, int& t);							//	求解贝祖等式 sa+tb=(a,b)
int Get_i_();																	//	获取进度参数i,方便操作
std::string factorize(long long num);										    //	分解因数
std::string Eular(std::string num);												//	欧拉函数
std::string Congruence_1(int a, int b, int m);									//	求解一次同余式,形如 ax ≡ b (mod m)
std::string add(std::string num1, std::string num2);							//	高精加
std::string subtraction(std::string num1, std::string num2);					//	高精减
std::string multiply(std::string num1, std::string num2);						//	高精乘

//	-------------- mathsenior -----------------
std::tuple<std::string, std::string> division2(std::string dividend, std::string divisor);	//	进阶高精除
std::tuple<std::string, std::string> division3(std::string dividend, std::string divisor);	//	进阶余正高精除

//	--------- config ---------
bool LoadConfigFile();			//	加载配置文件信息进内存
