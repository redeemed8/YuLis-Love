#define _CRT_SECURE_NO_WARNINGS
#include "head.h"
#include <iostream>
#include <stdio.h>

void ValidNumStr(std::string& numStr) {
	bool rollback = numStr[0] == '-' ? true : false;
	if (rollback)	numStr = numStr.substr(1, numStr.size() - 1);	//	先去除负号

	int index = -1;		//	表示妨碍有效数字的0 （也就是前置0）的最后一个下标  00000123 -> 5
	while (index + 1 < numStr.size() && numStr[index + 1] == '0')	++index;

	if (index + 1 == numStr.size())
		numStr = "0";
	else if (index == -1)
		numStr = rollback ? "-" + numStr : numStr;
	else
		numStr = rollback ? "-" + numStr.substr(index + 1, numStr.size() - index) : numStr.substr(index + 1, numStr.size() - index);
}

int compare(int a[], int b[]) {
	//索引为0的数据为数组长度
	if (a[0] > b[0])
		return 1;
	else if (a[0] < b[0])
		return -1;
	//逐位比较
	for (int i = a[0]; i > 0; i--) {
		if (a[i] > b[i])
			return 1;
		else if (a[i] < b[i])
			return -1;
	}
	return 0;
}

void numcpy(int a[], int b[], int dest) {
	//将数组右移，使两个数组右端对齐，形参q数组储存右移后的结果
	for (int i = 1; i <= a[0]; i++)		b[i + dest - 1] = a[i];
	b[0] = a[0] + dest - 1;
}

void StringToCharArray(std::string& str1, std::string& str2, char s1[], char s2[]) {
	//	这里s1 ,s2的长度最大为 5000，在此之前应该判断str1，str2的长度
	int len1 = str1.size(), len2 = str2.size();
	for (int i = 0; i < len1; ++i)		s1[i] = str1[i];
	for (int i = 0; i < len2; ++i)		s2[i] = str2[i];
	s1[len1] = '\0';
	s2[len2] = '\0';
	return;
}

void InitIntArray(int arr[], int count) {
	for (int i = 0; i < count; ++i)	arr[i] = 0;
}

#define MAXN 5004				//最大值。+4为了防止A+B出现进位

std::tuple<std::string, std::string> division2(std::string dividend, std::string divisor) {
	ValidNumStr(dividend);
	ValidNumStr(divisor);
	if (divisor == "0" || divisor == "-0") {
		return std::make_tuple("# The divisor cannot be 0!", "$ ");
	}
	char* s1 = new char[MAXN];				//存储字符串
	char* s2 = new char[MAXN];				//存储字符串
	int* tmp = new int[MAXN];				//交换用字符串
	int* a = new int[MAXN];					//存储加数A
	int* b = new int[MAXN];					//存储加数B
	int* c = new int[MAXN];					//存储和B

	InitIntArray(a, MAXN);
	InitIntArray(b, MAXN);
	InitIntArray(c, MAXN);

	StringToCharArray(dividend, divisor, s1, s2);

	std::string quotient;							//	存储商
	std::string remainder;							//	存储余数

	//1. 处理负数
	bool flaga = false;						//乘数a的符号
	if ('-' == s1[0]) {
		flaga = true;
		strcpy(s1, &s1[1]);					//删除负号
	}
	bool flagb = false;						//乘数b的符号
	if ('-' == s2[0]) {
		flagb = true;
		strcpy(s2, &s2[1]);					//删除负号
	}

	//2. 处理输出的负号
	if ((flaga ^ flagb) == 1) {
		quotient += "-";			//商为负数
	}

	//3. 处理乘数1
	int len = strlen(s1);
	a[0] = len;
	for (int i = 0; i < len; i++) {
		a[len - i] = s1[i] - '0';
	}

	//4. 处理乘数2
	len = strlen(s2);
	b[0] = len;
	for (int i = 0; i < len; i++) {
		b[len - i] = s2[i] - '0';
	}

	//5.做除法
	if (0 == compare(a, b)) {
		//	两数相等
		quotient = "1";
		remainder = "0";
		return std::make_tuple(quotient, remainder);
	}
	else if (-1 == compare(a, b)) {
		//	被除数小，除数大
		quotient = "0";
		remainder = dividend;
		return std::make_tuple(quotient, remainder);
	}
	else {
		c[0] = a[0] - b[0] + 1;
		for (int i = c[0]; i > 0; i--) {
			InitIntArray(tmp, MAXN);
			//高位对齐
			numcpy(b, tmp, i);

			while (compare(a, tmp) >= 0) {
				c[i]++;
				//减法
				for (int j = 1; j <= a[0]; j++) {
					if (a[j] < tmp[j]) {
						a[j + 1]--;
						a[j] += 10;
					}
					a[j] -= tmp[j];
				}

				int k = a[0];
				while (a[k] == 0) {
					k--;
				}
				a[0] = k;
			}
		}
		//控制最高位的0
		while (c[0] > 0 && c[c[0]] == 0) {
			c[0]--;
		}
	}

	//6.逆序存储到 string中
	for (int i = c[0]; i > 0; i--) {
		quotient += std::to_string(c[i]);
	}

	if (0 == a[0]) {
		remainder = "0";
	}
	else {
		if (true == flaga) {
			remainder += "-";
		}
		for (int i = a[0]; i > 0; i--) {
			remainder += std::to_string(a[i]);
		}
	}

	delete[] s1;
	delete[] s2;
	delete[] tmp;
	delete[] a;
	delete[] b;
	delete[] c;
	return std::make_tuple(quotient, remainder);
}

std::tuple<std::string, std::string> division3(std::string dividend, std::string divisor) {
	ValidNumStr(dividend);
	ValidNumStr(divisor);
	auto result = division2(dividend, divisor);
	std::string quotient = std::get<0>(result);
	std::string remainder = std::get<1>(result);
	if (remainder[0] == '-') {

		if (dividend[0] == '-' && divisor[0] != '-') {
			//	商 -= 1  余数 += 除数
			quotient = add(quotient, "-1");
			remainder = add(remainder, divisor);
		}
		else if (dividend[0] == '-' && divisor[0] == '-') {
			//	商 += 1  余数 += (-1) * 除数
			quotient = add(quotient, "1");
			remainder = add(remainder, divisor.substr(1, divisor.size() - 1));
		}

	}
	return std::make_tuple(quotient, remainder);
}

