// 12141755 임현호
// FInal Project

#include <iostream>
#include <chrono>
// input을 랜덤으로 받고싶을 때만 사용
// #include <random>
#include "knapsack_DP.h"
#include "knapsack_GA.h"
using namespace std;

int main() {
	cout << "12141755 임현호========================" << endl;
	// 아이템 개수
	int N;
	// 가방의 무게
	int W;
	cout << "input N, W : ";
	cin >> N >> W;

	// i번째 아이템의 무게 초기화
	long* Wi = new long[N + 1];
	for (int i = 0; i <= N; i++)
		Wi[i] = 0;

	// i번째 아이템의 가치 초기화
	long* Pi = new long[N + 1];
	for (int i = 0; i <= N; i++)
		Pi[i] = 0;


	//	input을 random 으로 받고 싶을때만 사용
//		random_device rd;
//		mt19937_64 rng(rd());
//		uniform_int_distribution<__int64> dist(1, 100);


	// N만큼 i번째 아이템의 무게와 가치 입력
	cout << "input W, P (N times)" << endl;
	for (int i = 1; i <= N; i++) {
		cin >> Wi[i] >> Pi[i];

		//	input을 random으로 받고 싶을때만 사용
//				Wi[i] = dist(rng);
//				Pi[i] = dist(rng);
//				cout << Wi[i] << " " << Pi[i] << " ";
	}

	// DP 시간측정
	std::chrono::system_clock::time_point start1 = std::chrono::system_clock::now();

	// knapsack DP 실행                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	long result_DP = knapsack_DP(Wi, Pi, N, W);

	chrono::duration<double> sec1 = chrono::system_clock::now() - start1;
	// milli초로 변환
	chrono::milliseconds mil1 = chrono::duration_cast<chrono::milliseconds>(sec1);


	// GA 시간측정
	std::chrono::system_clock::time_point start2 = std::chrono::system_clock::now();

	// knapsack GA 실행
	GA ga(N * 3);	// 개체 수는 item수의 3배로 설정
	ga.set_GA(Wi, Pi, N, W);	// 초기화
	long result_GA = ga.generate();

	chrono::duration<double> sec2 = chrono::system_clock::now() - start2;
	// milli초로 변환
	chrono::milliseconds mil2 = chrono::duration_cast<chrono::milliseconds>(sec2);

	// DP, GA 각각의 경우의 최대값 출력
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

	// DP, GA 각각의 경우의 실행시간 ms 단위로 출력
	cout << "DP : " << mil1.count() << " ms" << endl;
	cout << "GA : " << mil2.count() << " ms" << endl;

	// 동적 할당 해제
	delete[] Wi;
	delete[] Pi;

	// 종료
	return 0;
}