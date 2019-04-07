// 12141755 ����ȣ
// FInal Project

#include <iostream>
#include <chrono>
// input�� �������� �ް���� ���� ���
// #include <random>
#include "knapsack_DP.h"
#include "knapsack_GA.h"
using namespace std;

int main() {
	cout << "12141755 ����ȣ========================" << endl;
	// ������ ����
	int N;
	// ������ ����
	int W;
	cout << "input N, W : ";
	cin >> N >> W;

	// i��° �������� ���� �ʱ�ȭ
	long* Wi = new long[N + 1];
	for (int i = 0; i <= N; i++)
		Wi[i] = 0;

	// i��° �������� ��ġ �ʱ�ȭ
	long* Pi = new long[N + 1];
	for (int i = 0; i <= N; i++)
		Pi[i] = 0;


	//	input�� random ���� �ް� �������� ���
//		random_device rd;
//		mt19937_64 rng(rd());
//		uniform_int_distribution<__int64> dist(1, 100);


	// N��ŭ i��° �������� ���Կ� ��ġ �Է�
	cout << "input W, P (N times)" << endl;
	for (int i = 1; i <= N; i++) {
		cin >> Wi[i] >> Pi[i];

		//	input�� random���� �ް� �������� ���
//				Wi[i] = dist(rng);
//				Pi[i] = dist(rng);
//				cout << Wi[i] << " " << Pi[i] << " ";
	}

	// DP �ð�����
	std::chrono::system_clock::time_point start1 = std::chrono::system_clock::now();

	// knapsack DP ����                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	long result_DP = knapsack_DP(Wi, Pi, N, W);

	chrono::duration<double> sec1 = chrono::system_clock::now() - start1;
	// milli�ʷ� ��ȯ
	chrono::milliseconds mil1 = chrono::duration_cast<chrono::milliseconds>(sec1);


	// GA �ð�����
	std::chrono::system_clock::time_point start2 = std::chrono::system_clock::now();

	// knapsack GA ����
	GA ga(N * 3);	// ��ü ���� item���� 3��� ����
	ga.set_GA(Wi, Pi, N, W);	// �ʱ�ȭ
	long result_GA = ga.generate();

	chrono::duration<double> sec2 = chrono::system_clock::now() - start2;
	// milli�ʷ� ��ȯ
	chrono::milliseconds mil2 = chrono::duration_cast<chrono::milliseconds>(sec2);

	// DP, GA ������ ����� �ִ밪 ���
	cout << "DP : " << result_DP << endl;
	cout << "GA : " << result_GA << endl;

	cout << "GA bit : ";
	for (int i = 0; i < N; i++)
		cout << ga.best_gene.bit[i];
	cout << endl;

	cout << "Pi : ";
	for (int i = 0; i < N; i++)
		if (ga.best_gene.bit[i])
			cout << Pi[i + 1] << " ";
	cout << endl;

	cout << "Wi : ";
	for (int i = 0; i < N; i++)
		if (ga.best_gene.bit[i])
			cout << Wi[i + 1] << " ";
	cout << endl;

	// DP, GA ������ ����� ����ð� ms ������ ���
	cout << "DP : " << mil1.count() << " ms" << endl;
	cout << "GA : " << mil2.count() << " ms" << endl;

	// ���� �Ҵ� ����
	delete[] Wi;
	delete[] Pi;

	// ����
	return 0;
}