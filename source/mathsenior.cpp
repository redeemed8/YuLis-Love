#define _CRT_SECURE_NO_WARNINGS
#include "head.h"
#include <iostream>
#include <stdio.h>

void ValidNumStr(std::string& numStr) {
	bool rollback = numStr[0] == '-' ? true : false;
	if (rollback)	numStr = numStr.substr(1, numStr.size() - 1);	//	��ȥ������

	int index = -1;		//	��ʾ������Ч���ֵ�0 ��Ҳ����ǰ��0�������һ���±�  00000123 -> 5
	while (index + 1 < numStr.size() && numStr[index + 1] == '0')	++index;

	if (index + 1 == numStr.size())
		numStr = "0";
	else if (index == -1)
		numStr = rollback ? "-" + numStr : numStr;
	else
		numStr = rollback ? "-" + numStr.substr(index + 1, numStr.size() - index) : numStr.substr(index + 1, numStr.size() - index);
}

int compare(int a[], int b[]) {
	//����Ϊ0������Ϊ���鳤��
	if (a[0] > b[0])
		return 1;
	else if (a[0] < b[0])
		return -1;
	//��λ�Ƚ�
	for (int i = a[0]; i > 0; i--) {
		if (a[i] > b[i])
			return 1;
		else if (a[i] < b[i])
			return -1;
	}
	return 0;
}

void numcpy(int a[], int b[], int dest) {
	//���������ƣ�ʹ���������Ҷ˶��룬�β�q���鴢�����ƺ�Ľ��
	for (int i = 1; i <= a[0]; i++)		b[i + dest - 1] = a[i];
	b[0] = a[0] + dest - 1;
}

void StringToCharArray(std::string& str1, std::string& str2, char s1[], char s2[]) {
	//	����s1 ,s2�ĳ������Ϊ 5000���ڴ�֮ǰӦ���ж�str1��str2�ĳ���
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

#define MAXN 5004				//���ֵ��+4Ϊ�˷�ֹA+B���ֽ�λ

std::tuple<std::string, std::string> division2(std::string dividend, std::string divisor) {
	ValidNumStr(dividend);
	ValidNumStr(divisor);
	if (divisor == "0" || divisor == "-0") {
		return std::make_tuple("# The divisor cannot be 0!", "$ ");
	}
	char* s1 = new char[MAXN];				//�洢�ַ���
	char* s2 = new char[MAXN];				//�洢�ַ���
	int* tmp = new int[MAXN];				//�������ַ���
	int* a = new int[MAXN];					//�洢����A
	int* b = new int[MAXN];					//�洢����B
	int* c = new int[MAXN];					//�洢��B

	InitIntArray(a, MAXN);
	InitIntArray(b, MAXN);
	InitIntArray(c, MAXN);

	StringToCharArray(dividend, divisor, s1, s2);

	std::string quotient;							//	�洢��
	std::string remainder;							//	�洢����

	//1. ������
	bool flaga = false;						//����a�ķ���
	if ('-' == s1[0]) {
		flaga = true;
		strcpy(s1, &s1[1]);					//ɾ������
	}
	bool flagb = false;						//����b�ķ���
	if ('-' == s2[0]) {
		flagb = true;
		strcpy(s2, &s2[1]);					//ɾ������
	}

	//2. ��������ĸ���
	if ((flaga ^ flagb) == 1) {
		quotient += "-";			//��Ϊ����
	}

	//3. �������1
	int len = strlen(s1);
	a[0] = len;
	for (int i = 0; i < len; i++) {
		a[len - i] = s1[i] - '0';
	}

	//4. �������2
	len = strlen(s2);
	b[0] = len;
	for (int i = 0; i < len; i++) {
		b[len - i] = s2[i] - '0';
	}

	//5.������
	if (0 == compare(a, b)) {
		//	�������
		quotient = "1";
		remainder = "0";
		return std::make_tuple(quotient, remainder);
	}
	else if (-1 == compare(a, b)) {
		//	������С��������
		quotient = "0";
		remainder = dividend;
		return std::make_tuple(quotient, remainder);
	}
	else {
		c[0] = a[0] - b[0] + 1;
		for (int i = c[0]; i > 0; i--) {
			InitIntArray(tmp, MAXN);
			//��λ����
			numcpy(b, tmp, i);

			while (compare(a, tmp) >= 0) {
				c[i]++;
				//����
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
		//�������λ��0
		while (c[0] > 0 && c[c[0]] == 0) {
			c[0]--;
		}
	}

	//6.����洢�� string��
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
			//	�� -= 1  ���� += ����
			quotient = add(quotient, "-1");
			remainder = add(remainder, divisor);
		}
		else if (dividend[0] == '-' && divisor[0] == '-') {
			//	�� += 1  ���� += (-1) * ����
			quotient = add(quotient, "1");
			remainder = add(remainder, divisor.substr(1, divisor.size() - 1));
		}

	}
	return std::make_tuple(quotient, remainder);
}

