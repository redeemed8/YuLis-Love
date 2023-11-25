#include "head.h"

#define RADIX_INFORMAL "# The input parameters are not standardized, should be \"xx-to-xx\"   (xx) ~ [2, 16]!\n"

//	检查进制和进制串是否规范, 规范返回 "from-to",否则返回  "# 错误信息"
std::string CheckRadixStr(std::string radix, std::string radixNum) {
	char radixNums[17] = {
	'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','?'
	};
	//	使用正则表达式
	std::regex radix_pattern("\\b(?:[2-9]|1[0-6])-to-(?:[2-9]|1[0-6])\\b");
	bool isMatch = std::regex_match(radixNum, radix_pattern);
	if (!isMatch)	return RADIX_INFORMAL;

	std::string from("");
	for (int i = 0; i < radixNum.size(); ++i) {
		if (radixNum[i] == '-')		break;
		from += radixNum[i];
	}
	std::string to("");
	for (int i = radixNum.size() - 1; i >= 0; --i) {
		if (radixNum[i] == '-')		break;
		to = radixNum[i] + to;
	}
	int endIndex = std::stoi(from);
	for (char c : radix) {
		for (int i = 0; i <= endIndex; ++i) {
			if (i == endIndex)		return "# The input str '" + radix + "' and the input radix '" + from + "' do not match!\n";
			if (c == radixNums[i])	break;
		}
	}
	return "";
}

//	高精度10进制字符串加法
std::string add(const std::string& num1, const std::string& num2) {
	std::string result;

	// 将两个数字反转，从个位开始相加
	std::string reversedNum1(num1.rbegin(), num1.rend());
	std::string reversedNum2(num2.rbegin(), num2.rend());

	int carry = 0; // 进位

	// 逐位相加
	size_t end = reversedNum1.length() > reversedNum2.length() ? reversedNum1.length() : reversedNum2.length();
	for (size_t i = 0; i < end || carry; ++i) {
		int digit1 = i < reversedNum1.length() ? reversedNum1[i] - '0' : 0;
		int digit2 = i < reversedNum2.length() ? reversedNum2[i] - '0' : 0;

		int sum = digit1 + digit2 + carry;
		carry = sum / 10;

		// 将当前位的数字加到结果字符串的前面
		result.insert(result.begin(), '0' + sum % 10);
	}

	return result;
}

//	高精度10进制字符串减法
std::string subtraction(std::string& num1, std::string& num2) {
	std::string result;
	int borrow = 0;
	// 对齐两个数字的长度
	size_t len1 = num1.length();
	size_t len2 = num2.length();
	size_t maxLength = len1 > len2 ? len1 : len2;
	num1.insert(0, maxLength - len1, '0');
	num2.insert(0, maxLength - len2, '0');
	// 逐位进行减法
	for (int i = maxLength - 1; i >= 0; --i) {
		int digit1 = num1[i] - '0';
		int digit2 = num2[i] - '0';
		int diff = digit1 - digit2 - borrow;
		if (diff < 0) {
			diff += 10;
			borrow = 1;
		}
		else {
			borrow = 0;
		}
		result.insert(result.begin(), diff + '0');
	}
	// 去除结果前面的多余的0
	size_t pos = result.find_first_not_of('0');
	if (pos != std::string::npos) {
		return result.substr(pos);
	}
	return "0";
}

//	高精度10进制字符串乘法
std::string multiply(const std::string& num1, const std::string& num2) {
	int len1 = num1.size();
	int len2 = num2.size();
	std::string result(len1 + len2, '0');

	for (int i = len1 - 1; i >= 0; --i) {
		int carry = 0;

		for (int j = len2 - 1; j >= 0; --j) {
			int temp = (num1[i] - '0') * (num2[j] - '0') + (result[i + j + 1] - '0') + carry;
			result[i + j + 1] = temp % 10 + '0';
			carry = temp / 10;
		}

		result[i] += carry;
	}

	// 去除结果前面的多余的0
	size_t pos = result.find_first_not_of('0');
	if (pos != std::string::npos) {
		return result.substr(pos);
	}

	return "0";
}

// 高精度10进制字符串整数除法
std::string division(const std::string& dividend, int divisor) {
	std::string result;
	int remainder = 0;

	// 逐位进行除法
	for (char digit : dividend) {
		int currentDigit = digit - '0' + remainder * 10;
		result.push_back(currentDigit / divisor + '0');
		remainder = currentDigit % divisor;
	}

	// 移除结果前面的多余的0
	size_t pos = result.find_first_not_of('0');
	if (pos != std::string::npos) {
		return result.substr(pos);
	}

	return "0";
}

std::map<char, std::string > HexNumMap = {
	{'A',"10"},{'B',"11"},{'C',"12"},
	{'D',"13"},{'E',"14"},{'F',"15"}
};

//	任意进制转10进制, 正确返回 10进制串
std::string RadixToDecimal(std::string radix, std::string radixNum) {
	if (radixNum == "10")	return radix;
	std::string ret("0");		//	存储结果

	//	从最后一位开始遍历
	for (int i = radix.size() - 1; i >= 0; --i) {
		int times = radix.size() - 1 - i;				//	当前位的幂次数
		std::string radix_n("1");						//	幂的初值
		for (int j = 0; j < times; ++j) {
			radix_n = multiply(radix_n, radixNum);		//	得到相应的 n 次幂
		}
		//	再乘以当前位的系数
		char f = radix[i];
		std::string factor("");
		factor += f;
		if (f < '0' || f > '9') {
			factor = HexNumMap[f];
		}
		ret = add(ret, multiply(factor, radix_n));
	}

	return ret;
}

std::map<std::string, std::string > HexNumStrMap = {
	{"10","A"},{"11","B"},{"12","C"},
	{"13","D"},{"14","E"},{"15","F"}
};
//	10进制转任意进制, 正确返回 n 进制串
std::string DecimalToRadix(std::string decimal, std::string toRadixNum) {
	if (decimal == "0")		return decimal;
	int div = std::stoi(toRadixNum);		//	每次除数
	std::string* rets = new std::string[1000];		//	结果集合
	int count = 0;		//	结果长度

	while (decimal != "0") {
		std::string shang = division(decimal, div);
		std::string j = multiply(shang, toRadixNum);
		std::string yushu = subtraction(decimal, j);
		rets[count++] = yushu;
		decimal = shang;
	}

	for (int i = 0; i < count; ++i) {
		if (rets[i].size() > 1)		rets[i] = HexNumStrMap[rets[i]];
	}

	std::string ret("");
	for (int i = count - 1; i >= 0; --i) {
		ret += rets[i];
	}
	return ret;
}

//	任意两个进制之间的转换,正确返回 进制串, 错误返回 "# 错误信息"
std::string Radix1ToRedix2(std::string howToTrans, std::string radix) {
	//	howToTrans  xx-to-xx
	std::string from("");
	std::string to("");

	for (int i = 0; i < howToTrans.size(); ++i) {
		if (howToTrans[i] == '-')	break;
		from += howToTrans[i];
	}
	for (int i = howToTrans.size() - 1; i >= 0; --i) {
		if (howToTrans[i] == '-')	break;
		to = howToTrans[i] + to;
	}
	std::string dec = RadixToDecimal(radix, from);
	std::string ret = DecimalToRadix(dec, to);

	return ret;
}

#define STR_NONUMBER "# The format is wrong and you should enter several numbers!\n"

//	检查一个字符串是不是数字,是返回该数字，否则返回错误信息 "# 错误信息"
std::string CheckStrIsNumber(std::string& str) {
	if (str.size() == 1 && (str[0] < '0' || str[0]>'9'))	return STR_NONUMBER;
	int flag = 1;		//	1 为 >= 0 ,  0 为 < 0	,	默认为大于0
	if (str[0] == '-') {
		flag = 0;
		str = str.substr(1, str.size() - 1);
	}
	else if (str[0] == '+')
		str = str.substr(1, str.size() - 1);
	for (char c : str) {
		if (c < '0' || c>'9')
			return STR_NONUMBER;
	}
	return flag ? str : "-" + str;
}

void AbsNumstr(std::string& str) {
	str = str[0] == '-' || str[0] == '+' ? str.substr(1, str.size() - 1) : str;
}

//	比较两个 正数数字 字符串内的数字
//	str1大于str1 返回1， str1小于str2 返回-1， str1等于str2返回0
int NumstrGreater(std::string& str1, std::string& str2) {
	if (str1 == str2)	return 0;
	if (str1.size() > str2.size())	return 1;
	if (str1.size() < str2.size())	return -1;
	//	长度相等，逐位比较
	for (int i = 0; i < str1.size(); ++i) {
		if (str1[i] > str2[i])	return 1;
		if (str1[i] < str2[i])	return -1;
	}
	return 0;
}

//	判断数字字符串是否大于 int_max
bool IsGreatIntMax(std::string str) {
	if (str.size() > 10) return true;
	if (str.size() < 10) return false;
	std::string int_max = "2147483647";
	return NumstrGreater(str, int_max) == 1;
}

//	判断数字字符串是否大于 long_long_max
bool IsGreatLongLongMax(std::string str) {
	if (str.size() > 19)	return true;
	if (str.size() < 19)	return false;
	std::string long_long_max = "9223372036854775807";
	return NumstrGreater(str, long_long_max) == 1;
}

//	最大公因数--10进制
std::string MaxCommonFactor(std::string& num1, std::string& num2) {
	//	在调用前，应用CheckStrIsNumber检查其参数是否为数字串
	AbsNumstr(num1);
	AbsNumstr(num2);
	if (num1 == "0" && num2 == "0")
		return "# (0, 0)未定义!";
	else if (num1 == "1" || num1 == "0")
		return num2;
	else if (num2 == "1" || num2 == "0")
		return num1;
	std::string large, tiny;
	//	取大数作为 (a, b)中的 a，小数作为 b
	if (NumstrGreater(num1, num2) == 1) {
		large = num1;
		tiny = num2;
	}
	else if (NumstrGreater(num1, num2) == -1) {
		large = num2;
		tiny = num1;
	}
	else {
		return "1";
	}

	//	将小数化为int
	if (IsGreatIntMax(tiny)) {
		return "# The lesser of the two numbers is too large, should be less than 2147483647";
	}
	int tiny_num = std::stoi(tiny);

	while (tiny_num != 0) {
		int tmp = tiny_num;
		std::string shang = division(large, tiny_num);
		std::string jianshu = multiply(shang, std::to_string(tiny_num));
		tiny_num = std::stoi(subtraction(large, jianshu));
		large = std::to_string(tmp);
	}
	return large;
}

#define NO_NEGATIVE "# Don't want a negative number in the number you enter!"

//	最小公倍数--10进制
std::string MinCommonMultiples(std::string& num1, std::string& num2) {
	if (num1[0] == '-' || num2[0] == '-')	return NO_NEGATIVE;
	AbsNumstr(num1);
	AbsNumstr(num2);
	if (num1 == "0" || num2 == "0")	return "0";
	if (num1 == "1")	return num2;
	if (num2 == "1")		return num1;
	std::string mcf = MaxCommonFactor(num1, num2);
	if (mcf[0] == '#')	return mcf;
	std::string num1_num2_ji = multiply(num1, num2);
	return division(num1_num2_ji, std::stoi(mcf));
}

//	扩展欧几里得除法，求贝祖等式 sa + tb = (a, b)
void ExtendedEuclidean(int a, int b, int& s, int& t) {
	if (b == 0) {
		// 基本情况：当b为0时，a即为最大公约数，x为1，y为0
		s = 1;
		t = 0;
	}
	else {
		// 递归调用扩展欧几里得算法
		ExtendedEuclidean(b, a % b, s, t);
		// 更新x和y的值
		int temp = s;
		s = t;
		t = temp - (a / b) * t;
	}
}

int _i_ = 2;
int Get_i_() {
	return _i_;
}

//	分解因数, 成功后返回结果串,否则返回 "# 错误信息"
std::string factorize(long long num) {
	//	在这里之前，应该判断数字的范围
	if (num == 0L || num == 1L)	return std::to_string(num);
	if (num < 0)	num = (-1) * num;

	std::string factors;
	for (_i_ = 2; _i_ * _i_ <= num; ++_i_) {
		int count = 0;
		while (num % _i_ == 0) {
			++count;
			num /= _i_;
		}
		if (count > 0)	factors += std::to_string(_i_) + "^" + std::to_string(count) + " ";
	}
	if (num > 1)	factors += std::to_string(num) + "^1";
	return factors;
}

struct Result {
	std::vector<int> rets;
	std::string error;
};

//	分析因数串
Result AnalyseFactorStr(std::string factorStr) {
	Result result;
	if (factorStr[0] == '#') {
		result.error = factorStr;
		return result;
	}
	std::set<std::string> temp;
	factorStr = " " + factorStr;

	std::regex pattern1("(\\d+)\\^");		// 匹配指数符号前的数字
	std::regex pattern2("\\s+(\\d+)");		// 匹配前有空格的数字

	std::sregex_iterator it1(factorStr.begin(), factorStr.end(), pattern1);
	std::sregex_iterator end1;
	std::sregex_iterator it2(factorStr.begin(), factorStr.end(), pattern2);
	std::sregex_iterator end2;

	while (it1 != end1) {
		std::smatch match = *it1;
		temp.insert(match[1].str());
		++it1;
	}
	while (it2 != end2) {
		std::smatch match = *it2;
		temp.insert(match[1].str());
		++it2;
	}

	for (auto it = temp.begin(); it != temp.end(); ++it) {
		if (IsGreatIntMax(*it)) {
			result.error = "# The factor is too large, should be less than 2147483647";
			return result;
		}
		result.rets.push_back(std::stoi(*it));
	}
	result.error = "";
	return result;
}

//	num串的n次幂
std::string N_power(std::string num, int n) {
	if (n < 0)	return "# The value of the power cannot be negative!";
	if (n == 0)	return "1";
	if (n == 1)	return num;
	std::string ret = "1";
	for (int i = 0; i < n; ++i)		ret = multiply(ret, num);
	return ret;
}

std::string EularSenior(std::string factorStr);
//	欧拉函数
std::string Eular(std::string num) {
	//	1.判断是不是数字
	//	2.判断数字范围

	if (num == "1")	return "1";

	std::string factorStr = factorize(std::stoll(num));
	Result r = AnalyseFactorStr(factorStr);

	if (r.error != "" && r.error[0] == '#')		return r.error;

	if (r.rets.size() == 1 && num == std::to_string(r.rets[0])) {
		return std::to_string(r.rets[0] - 1);
	}

	//	乘所有分子
	for (int number : r.rets) {
		num = multiply(num, std::to_string(number - 1));
	}

	//	除以所有分母
	for (int number : r.rets) {
		size_t dp = num.find('.');
		if (dp != std::string::npos) {
			num = num.substr(0, dp);
		}
		num = division(num, number);
		if (CheckStrIsNumber(num)[0] == '#') {
			return EularSenior(factorStr);
		}
	}
	return num;
}

//	取出因数串对应的底数和幂数
std::map<std::string, std::string> AnalyseFactorStr2(std::string factorStr) {
	std::map<std::string, std::string> base_power;

	std::regex pattern("(\\d+)\\^(\\d+)");		// 匹配数字和^之间的内容
	auto words_begin = std::sregex_iterator(factorStr.begin(), factorStr.end(), pattern);
	auto words_end = std::sregex_iterator();

	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		std::smatch match = *i;
		base_power[match[1]] = match[2];
	}
	return base_power;
}

//	高级欧拉
std::string EularSenior(std::string factorStr) {
	//	得到底幂对
	std::map<std::string, std::string> base_power = AnalyseFactorStr2(factorStr);

	std::string ret = "1";
	for (auto const& pair : base_power) {
		std::string j1 = N_power(pair.first, std::stoi(pair.second));
		std::string j2 = N_power(pair.first, std::stoi(pair.second) - 1);
		std::string m = subtraction(j1, j2);
		ret = multiply(ret, m);
	}
	return ret;

}

//	求 a mod m 的逆元 aa' ≡ 1 (mod m)
std::string Inverse_element(int a, int m) {
	if (IsGreatIntMax(std::to_string(a)) || IsGreatIntMax(std::to_string(m))) {
		return "# The values of a and m cannot be greater than 2147483647!";
	}
	int s, t;
	ExtendedEuclidean(a, m, s, t);
	return std::to_string(s % m > 0 ? s % m : m + (s % m));
}

//	求解一次同余式,形如 ax ≡ b (mod m)
std::string Congruence_1(int a, int b, int m) {
	//	判断参数条件
	if (m < 1)	return "# The value of m should be a positive integer!";
	if (a % m == 0)	return"# The parameter m is divisible by a! No solution!";
	b = b < 0 ? m + b : b;
	std::string a_str = std::to_string(a);
	std::string b_str = std::to_string(b);
	std::string m_str = std::to_string(m);

	std::string a_m_str = MaxCommonFactor(a_str, m_str);

	if (a_m_str[0] == '#')	return a_m_str;

	int a_m = std::stoi(a_m_str);

	//	求逆元
	std::string ie = Inverse_element(a / a_m, m / a_m);
	if (ie[0] == '#')	return ie;

	//	判断有几个解,有(a,m)个解	t = 0, 1, 2, ... , (a,m)-1

	std::string rets = "x ≡ ";
	std::string b_am = std::to_string(b / a_m);
	std::string m_am = std::to_string(m / a_m);
	for (int i = 0; i < a_m; ++i) {
		std::string add1 = multiply(b_am, ie);
		std::string add2 = multiply(m_am, std::to_string(i));
		std::string Total = add(add1, add2);

		std::string shang = division(Total, m);
		std::string j = multiply(shang, std::to_string(m));
		std::string ret = subtraction(Total, j);
		rets += ret;
		if (i != a_m - 1)	rets += ", ";
	}
	return rets + " (mod " + m_str + ")";
}



