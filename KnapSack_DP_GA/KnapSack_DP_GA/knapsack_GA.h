#ifndef KNAPSACK_GA_H
#define KNAPSACK_GA_H

#include <iostream>
#include <vector>
#include <random>
using namespace std;

#define NUM 200		// 개체 수
#define EPOCH 100	// 반복 수
#define CROSS 90	// Cross over 확률 (%)
#define MUTATE 1	// Mutation 확률 (%)
#define WEIGHT 10	// 무게 가중치

// 유전자 struct
typedef struct Gene {
	// bit 저장
	vector<int> bit;

	// bit size 저장
	int size = 0;

	// fit 저장
	long fit = 1;

	// 기본 생성자
	Gene() { }

	// random bit 생성
	void initialize(int n) {
		size = n;	// 비트 수

		// 난수 생성을 위한 절차
		random_device rd;
		mt19937_64 rng(rd());
		for (int i = 0; i < size; i++) {
			uniform_int_distribution<__int64> dist(0, 1);	// 0 ~ 1 정수 난수 생성
			bit.push_back(dist(rng));
		}
	}

	// 연산자 오버로딩
	bool operator <(const Gene &g) const {
		return (fit < g.fit);
	}
	bool operator >(const Gene &g) const {
		return (fit > g.fit);
	}
	bool operator ==(const Gene &g) const {
		for (int i = 0; i < size; i++) {
			if (bit[i] != g.bit[i])
				return false;
		}
		return true;
	}
}Gene;

// knapsack GA class
class GA {
private:
	int epoch;	// 반복 수
	int num;	// 개체 수
	long* Wi;	// i번째 아이템의 무게
	long* Pi;	// i번째 아이템의 가치
	int N;		// 아이템의 수
	int W;		// 가방 최대 무게
	vector<Gene> gene;	// 유전자 저장하는 vector
	long best_price = 0;	// 최대값 저장하는 변수

	void calculate_gene();	// 각 유전자의 값 계산
	void make_gene();	// 유전자 최초 생성

public:
	Gene best_gene;	// 최대값에 해당하는 비트

	// 생성자
	GA(int num_ = NUM, int epoch_ = EPOCH) {
		epoch = epoch_;
		num = num_;
	}
	void set_GA(long* Wi_, long* Pi_, int N_, int W_);	// 초기화 세팅 함수
	long generate();	// 세대 진행 후 최대값 반환
};

void GA::set_GA(long* Wi_, long* Pi_, int N_, int W_) {
	// N, W 받아옴
	N = N_;
	W = W_;

	// i번째 아이템의 무게 초기화
	Wi = new long[N + 1];
	for (int i = 0; i <= N; i++)
		Wi[i] = 0;

	// i번째 아이템의 가치 초기화
	Pi = new long[N + 1];
	for (int i = 0; i <= N; i++)
		Pi[i] = 0;

	// Wi, Pi 받아옴
	for (int i = 1; i <= N; i++) {
		Wi[i] = Wi_[i];
		Pi[i] = Pi_[i];
	}

	// 유전자 random 초기화
	make_gene();

	// best 유전자 초기화
	best_gene.initialize(N);

	// 첫 번째 계산
	calculate_gene();
}

// 각 유전자를 random bit로 만들고 유전자 벡터에 삽입
void GA::make_gene() {
	for (int i = 0; i < num; i++) {
		Gene temp;
		temp.initialize(N);
		gene.push_back(temp);
	}
}

long GA::generate() {
	// select, cross over, mutate 진행
	for (int kk = 1; kk < epoch; kk++) {
		// 난수 생성 절차
		random_device rd;
		mt19937_64 rng(rd());
		uniform_int_distribution<__int64> dist(0, num - 1);
		uniform_int_distribution<__int64> dist2(1, 100);
		uniform_int_distribution<__int64> dist3(0, N - 1);

		vector<Gene> temp_gene;	// 현재 유전자 저장 벡터
		for (int i = 0; i < num; i++)
			temp_gene.push_back(gene[i]);

		// 교배할 부모 유전자 index
		int index_dad = 0;
		int index_mom = 0;

		for (int i = 0; i < num; i++) {
			// tournament select
			// a, b 난수 두개 생성 ( 0 ~ num-1 )
			int a = dist(rng);
			int b = dist(rng);

			// select dad
			if (i % 2 == 0) {
				// 해당 유전자 중 fitness가 높은걸로 설정
				if (gene[a] < gene[b])
					index_dad = b;
				else
					index_dad = a;
			}
			// select mom
			else {
				// 해당 유전자 중 fitness가 높은걸로 설정
				if (gene[a] < gene[b])
					index_mom = b;
				else
					index_mom = a;
			}

			// 부모를 둘다 고른 경우만 해당
			if (i % 2 != 0) {
				// cross over
				if (dist2(rng) <= CROSS) {
					// 고른 두 유전자가 같은 경우는 교배가 의미가 없다.
					// 하나는 그대로 저장되며, 나머지 하나는 random 형성한다. (다형성 위해서)
					if (temp_gene[index_dad] == temp_gene[index_mom]) {
						// random 유전자 생성
						Gene temp;
						temp.initialize(N);
						for (int j = 0; j < N; j++) {
							gene[i - 1].bit[j] = temp_gene[index_dad].bit[j];	// 하나는 그대로 저장
							gene[i].bit[j] = temp.bit[j];	// 하나는 random 저장
						}
					}
					// 두 유전자가 다른 경우
					else {
						// uniform cross over
						for (int j = 0; j < N; j++) {
							// 1 ~ 100 난수 생성
							int ran = dist2(rng);
							// 1/2 의 확률로 해당 비트 교배
							if (ran > 50) {
								gene[i - 1].bit[j] = temp_gene[index_dad].bit[j];
								gene[i].bit[j] = temp_gene[index_mom].bit[j];
							}
							// 나머지 절반의 확률로는 교배 하지 않음
							else {
								gene[i - 1].bit[j] = temp_gene[index_mom].bit[j];
								gene[i].bit[j] = temp_gene[index_dad].bit[j];
							}
						}
					}
				}
				// Cross over 되지 않는 경우 그대로 저장
				else {
					for (int j = 0; j < N; j++) {
						gene[i - 1].bit[j] = temp_gene[index_dad].bit[j];
						gene[i].bit[j] = temp_gene[index_mom].bit[j];
					}
				}
				// insert mutation
				// first child
				if (dist2(rng) <= MUTATE) {
					// 두개의 비트를 선택후 자리 변경
					int a = dist3(rng);
					int b = dist3(rng);
					int temp = gene[i - 1].bit[a];
					gene[i - 1].bit[a] = gene[i - 1].bit[b];
					gene[i - 1].bit[b] = temp;
				}
				// second child
				if (dist2(rng) <= MUTATE) {
					// 두개의 비트를 선택후 자리 변경
					int a = dist3(rng);
					int b = dist3(rng);
					int temp = gene[i].bit[a];
					gene[i].bit[a] = gene[i].bit[b];
					gene[i].bit[b] = temp;
				}
			}
		}
		// 교배, 돌연변이가 끝났으므로 다시 계산
		calculate_gene();
	}
	return best_price;
}

// 적합도 계산하는 함수
void GA::calculate_gene() {
	for (int i = 0; i < num; i++) {
		long sum_P = 0;	// 가치 합 저장하는 변수
		long sum_W = 0;	// 무게 합 저장하는 변수

		for (int j = 0; j < N; j++) {
			// 해당 비트가 1인 경우 합함
			if (gene[i].bit[j]) {
				sum_P += Pi[j + 1];
				sum_W += Wi[j + 1];
			}
		}
		gene[i].fit = sum_P;	// 적합도는 가치의 합
		// 무게를 초과한 경우
		if (sum_W > W)
			gene[i].fit -= WEIGHT * (sum_W - W);	// 초과한 무게만큼 가중치를 크게주어 적합도를 감소시킴
		// 무게를 초과하지 않은 경우
		else {
			// 최대값보다 가치의 합이 더 큰경우
			if (best_price < sum_P) {
				best_price = sum_P;	// 최대값을 교체해준다.
				for (int j = 0; j < N; j++)
					best_gene.bit[j] = gene[i].bit[j];
			}
		}
	}
}

#endif