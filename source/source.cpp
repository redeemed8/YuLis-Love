#define _CRT_SECURE_NO_WARNINGS
#include "head.h"

//	每个函数的参数最少数量
std::map <std::string, int> FuncArgsMap = {
	{"help",0},
	{"ChUpper",1},
	{"ChLower",1},
	{"Radix",2},
	{"MCF",2},
	{"MCM",2},
	{"Bezout",2},
	{"Factor",1},
	{"Eular",1},
	{"Congruence_1",3}
};

std::map<std::string, int> FuncMap = {
	{"help",0},
	{"ChLower",1},
	{"ChUpper",2},
	{"Radix",3},
	{"MCF",4},
	{"MCM",5},
	{"Bezout",6},
	{"Factor",7},
	{"Eular",8},
	{"Congruence_1",9}
};

std::vector<std::any> Vector;
//	Process默认构造
Process::Process() : Start(0.00f), End(0.00f), CurValue(0.00f), Result(Vector), Finish(false), Mode(0) { }

//	Process有参构造
Process::Process(double start, double end) :
	Start(start), End(end), CurValue(0.00f), Result(Vector), Finish(false), Mode(0) { }

//	延迟相同时间，达到几乎同步
void Process::Delay() {
	Sleep(PROCESS_DELAY);
}

//	查看当前进度
double Process::CheckCurProcess() {
	return (this->CurValue - this->Start) / (this->End - this->Start);
}

//	获取终止值
double Process::GetEnd() {
	return this->End;
}

//	设置中止值
bool Process::SetEnd(double _end) {
	this->End = _end;
	return this->End = _end;
}

//	修改模式
bool Process::SetMode(int _mode) {
	this->Mode = _mode;
	return this->Mode == _mode;
}

//	修改完成状态
bool Process::SetFinish(bool isFinish) {
	this->Finish = isFinish;
	return this->Finish = isFinish;
}

//	是否完成
bool Process::Finished() {
	return this->Finish;
}

//	给进度结果容器添加元素
bool Process::AddResults(std::any& a) {
	int pre = this->CurValue;
	this->Result.push_back(a);
	return this->Result.size() > pre;
}

Config::Config() : Sys("windows") { }

//	ProcessFactory创建Process对象
Process* ProcessFactory::Create_Process(double start, double end) {
	return new Process(start, end);
}

std::string ProcessSample = "|====================================================================================================> 100.00%";
//	Process持续更新进度,
bool Process::UpdateProcess() {
	bool FinishedOrOutput = this->Mode == 1 ? true : false;
	while (!this->Finish) {
		double curValue = this->Result.size();
		this->CurValue = curValue;
		if (curValue < this->Start) {
			return false;	//	false代表范围错误
		}
		if (curValue >= this->End || this->Finish) {
			Blue(ProcessSample + "\n");
			if (!FinishedOrOutput || !this->Result.size()) {
				this->Finish = true;
				return true;
			}
			Yellow("------------------- result ---------------------\n");
			for (std::any r : this->Result) {
				if (r.has_value()) {
					if (r.type() == typeid(std::string) && std::any_cast<std::string>(r)[0] == '$') {
						continue;
					}
					Yellow("-- ");
					ParseAnyAndPrint(r);
				}
			}
			Yellow("------------------------------------------------\n");
			this->Result.clear();
			this->Finish = true;
			return true;
		}
		Blue("|");								//	画一个  "|"

		double allProcess = this->End - this->Start;		//	所有进程
		double curProcess = curValue - this->Start;			//	当前进程
		int finish = (int)(curValue * 100 / allProcess);			//	当前进度
		int j;
		for (j = 0; j < finish; ++j) {
			Blue("=");
		}
		for (j = 0; j < 100 - finish; ++j) {
			std::cout << ' ';
		}
		Blue("> " + ToString(curValue * 100 / allProcess) + "%");		//	补齐箭头和百分比，一共长度为110
		Sleep(PROCESS_DELAY);	//	短暂延迟
		for (j = 0; j < PROCESSLEN; ++j) {
			std::cout << "\b";
		}
	}
}

//	将 0 ~ 999转换为字符串
std::string ToString(double num) {
	if (num < 0.00f || num > 999.99f) {
		return "";
	}
	char chs[7];		//	100.00\0
	sprintf(chs, "%6.2f", num);
	std::string str = std::string(chs);
	int len = str.size();
	if (len < 6) {
		for (int j = 0; j < 6 - len; ++j) {
			str = " " + str;
		}
	}
	return str;
}

//	将any对象转换为对应的类型后打印输出
void ParseAnyAndPrint(std::any a) {
	Yellow();
	if (a.type() == typeid(std::string)) {
		if (std::any_cast<std::string>(a)[0] == '$') {
			White();
			return;
		}
		else if (std::any_cast<std::string>(a)[0] == '#') {
			Red(std::any_cast<std::string>(a) + "\n");
		}
		else {
			std::cout << std::any_cast<std::string>(a) << '\n';
		}
	}
	else if (a.type() == typeid(double)) {
		std::cout << std::any_cast<double>(a) << '\n';
	}
	else if (a.type() == typeid(int)) {
		std::cout << std::any_cast<int>(a) << '\n';
	}
	White();
}

//	并发执行进度更新
void RunUpdateProcess(Process* p) {
	std::thread th(
		[p] {
			(*p).UpdateProcess();
		}
	);
	th.detach();
}

std::string AnalyseCommand(std::string command) {	//	分析命令,返回信息,成功无误返回空字符串
	if (!FuncArgsMap.count(command))		return UNKNOWN_COMMAND;
	if (GetCommandArgsNum() < FuncArgsMap[command])	return ARGS_NOENOUGH;
	return "";
}

//	监听_i_并更新
void ListenedUpdate(Process* p) {
	int temp = Get_i_();
	std::any aaa = std::any(std::string("$null"));
	while (1) {
		if (temp != Get_i_() && (*p).GetEnd() != 0.00f) {
			(*p).AddResults(aaa);
			temp = Get_i_();
		}
		Sleep(1);
	}
}


#define OUT_LONGLONG "# The lesser of the two numbers is too large, should be less than 9,223,372,036,854,775,807"

//	执行命令
void ExecuteCommand(std::string command) {
	ProcessFactory processFactory;
	Process* p = processFactory.Create_Process(0, GetCommandArgsNum());
	(*p).SetMode(PROCESS_OUTPUT);

	bool flag = false;
	int FuncId = FuncMap[command];

	if (FuncId == 0) {
		help();
	}
	else if (FuncId == 1 || FuncId == 2) {
		RunUpdateProcess(p);			//	启动
		flag = true;
		//	获取并遍历命令行传入的参数
		for (int i = 0; i < GetCommandArgsNum(); ++i) {
			std::string str = TransformCase(GetCommandArgs(i), FuncId);
			std::any a = str;
			(*p).AddResults(a);
		}
	}
	else if (FuncId == 3) {
		std::string isMatch = CheckRadixStr(GetCommandArgs(1), GetCommandArgs(0));
		if (isMatch[0] == '#') {
			Red(isMatch);
			PauseExit(1);
		}
		(*p).SetEnd(1.00f);
		RunUpdateProcess(p);
		flag = true;
		std::string ret = Radix1ToRedix2(GetCommandArgs(0), GetCommandArgs(1));
		std::any a = ret;
		(*p).AddResults(a);
	}
	else if (FuncId == 4) {
		for (int i = 0; i < 2; ++i) {
			std::string r = GetCommandArgs(i);
			std::string isMatch = CheckStrIsNumber(r);
			if (isMatch[0] == '#') {
				Red(isMatch);
				PauseExit(1);
			}
		}
		(*p).SetEnd(1.00f);
		RunUpdateProcess(p);
		flag = true;
		std::string a0 = GetCommandArgs(0);
		std::string a1 = GetCommandArgs(1);
		std::string ret = MaxCommonFactor(a0, a1);
		std::any a = ret;
		(*p).AddResults(a);
	}
	else if (FuncId == 5) {
		for (int i = 0; i < 2; ++i) {
			std::string r = GetCommandArgs(i);
			std::string isMatch = CheckStrIsNumber(r);
			if (isMatch[0] == '#') {
				Red(isMatch);
				PauseExit(1);
			}
		}
		(*p).SetEnd(1.00f);
		RunUpdateProcess(p);
		flag = true;
		std::string a0 = GetCommandArgs(0);
		std::string a1 = GetCommandArgs(1);
		std::string ret = MinCommonMultiples(a0, a1);
		std::any a = ret;
		(*p).AddResults(a);
	}
	else if (FuncId == 6) {
		for (int i = 0; i < 2; ++i) {
			std::string r = GetCommandArgs(i);
			std::string isMatch = CheckStrIsNumber(r);
			if (isMatch[0] == '#') {
				Red(isMatch);
				PauseExit(1);
			}
		}
		if (IsGreatIntMax(GetCommandArgs(0)) || IsGreatIntMax(GetCommandArgs(1))) {
			Red("# The lesser of the two numbers is too large, should be less than 2,147,483,647\n");
			PauseExit(1);
		}
		(*p).SetEnd(2.00f);
		RunUpdateProcess(p);
		flag = true;
		int s, t;
		int a = std::stoi(GetCommandArgs(0)), b = std::stoi(GetCommandArgs(1));
		ExtendedEuclidean(a, b, s, t);
		if (s * a + t * b < 0) {
			s = (-1) * s;
			t = (-1) * t;
		}
		std::any a1 = s;
		std::any a2 = t;
		(*p).AddResults(a1);
		(*p).AddResults(a2);
	}
	else if (FuncId == 7) {
		std::string numStr = GetCommandArgs(0);
		std::string isMatch = CheckStrIsNumber(numStr);
		if (isMatch[0] == '#') {
			Red(isMatch);
			PauseExit(1);
		}
		numStr = isMatch[0] == '-' ? isMatch.substr(1, isMatch.size() - 1) : isMatch;
		if (IsGreatLongLongMax(numStr)) {
			Red("# The lesser of the two numbers is too large, should be less than 9,223,372,036,854,775,807\n");
			PauseExit(1);
		}
		long long num = std::stoll(numStr);

		double end = std::sqrt(static_cast<double>(num));
		(*p).SetEnd(end);
		RunUpdateProcess(p);
		flag = true;

		std::thread th1(
			[p] {
				ListenedUpdate(p);
			}
		);
		th1.detach();

		std::string factors = factorize(num);
		std::any a = factors;
		(*p).AddResults(a);
		(*p).SetEnd(0.00f);
	}
	else if (FuncId == 8) {
		std::string numStr = GetCommandArgs(0);
		std::string isMatch = CheckStrIsNumber(numStr);
		if (isMatch[0] == '#') {
			Red(isMatch);
			PauseExit(1);
		}
		if (isMatch[0] == '-' || isMatch[0] == '0') {
			Red("# The argument of the Euler function needs to be a positive integer\n");
			PauseExit(1);
		}

		if (IsGreatLongLongMax(isMatch)) {
			Red(std::string(OUT_LONGLONG) + "\n");
			PauseExit(1);
		}

		(*p).SetEnd(1.00f);
		RunUpdateProcess(p);
		flag = true;

		std::string ola = Eular(isMatch);

		std::any a = ola;
		(*p).AddResults(a);
	}
	else if (FuncId == 9) {
		std::string abm[3] = { GetCommandArgs(0),GetCommandArgs(1),GetCommandArgs(2) };

		for (int i = 0; i < 3; ++i) {
			std::string check = CheckStrIsNumber(abm[i]);
			if (check[0] == '#') {
				Red(check);
				PauseExit(1);
			}
			abm[i] = check;
		}
		for (int i = 0; i < 3; ++i) {
			if (IsGreatIntMax(abm[i])) {
				Red("# The values of a and m cannot be greater than 2147483647! \n");
				PauseExit(1);
			}
		}

		(*p).SetEnd(1.00f);
		RunUpdateProcess(p);
		flag = true;

		std::string r = Congruence_1(std::stoi(abm[0]), std::stoi(abm[1]), std::stoi(abm[2]));
		std::any a = r;
		(*p).AddResults(a);
	}
	else {
		Red("[ Error ]: The execution process is abnormal! \n");
		PauseExit(1);
	}

	while (flag && !(*p).Finished()) {
		(*p).Delay();
	}

}
