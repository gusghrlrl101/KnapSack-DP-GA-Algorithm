#ifndef KNAPSACK_DP_H
#define KNAPSACK_DP_H

#include <iostream>
using namespace std;

long knapsack_DP(long* Wi, long* Pi, int N, int W) {
	// W ũ���� DP �迭 ����
	long* DP = new long[W + 1];

	// DP �迭 �ʱ�ȭ
	for (int i = 0; i <= W; i++)
		DP[i] = 0;

	// DP �ݺ���
	for (int i = 1; i <= N; i++) {
		for (int j = W; j >= Wi[i]; j--) {
			if (DP[j] < DP[j - Wi[i]] + Pi[i])
				DP[j] = DP[j - Wi[i]] + Pi[i];
		}
	}
	return DP[W];
	cout << "hyunho!" << endl;
}

#endif