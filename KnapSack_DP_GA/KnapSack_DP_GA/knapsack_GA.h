#ifndef KNAPSACK_GA_H
#define KNAPSACK_GA_H

#include <iostream>
#include <vector>
#include <random>
using namespace std;

#define NUM 200		// ��ü ��
#define EPOCH 100	// �ݺ� ��
#define CROSS 90	// Cross over Ȯ�� (%)
#define MUTATE 1	// Mutation Ȯ�� (%)
#define WEIGHT 10	// ���� ����ġ

// ������ struct
typedef struct Gene {
	// bit ����
	vector<int> bit;

	// bit size ����
	int size = 0;

	// fit ����
	long fit = 1;

	// �⺻ ������
	Gene() { }

	// random bit ����
	void initialize(int n) {
		size = n;	// ��Ʈ ��

		// ���� ������ ���� ����
		random_device rd;
		mt19937_64 rng(rd());
		for (int i = 0; i < size; i++) {
			uniform_int_distribution<__int64> dist(0, 1);	// 0 ~ 1 ���� ���� ����
			bit.push_back(dist(rng));
		}
	}

	// ������ �����ε�
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
	int epoch;	// �ݺ� ��
	int num;	// ��ü ��
	long* Wi;	// i��° �������� ����
	long* Pi;	// i��° �������� ��ġ
	int N;		// �������� ��
	int W;		// ���� �ִ� ����
	vector<Gene> gene;	// ������ �����ϴ� vector
	long best_price = 0;	// �ִ밪 �����ϴ� ����

	void calculate_gene();	// �� �������� �� ���
	void make_gene();	// ������ ���� ����

public:
	Gene best_gene;	// �ִ밪�� �ش��ϴ� ��Ʈ

	// ������
	GA(int num_ = NUM, int epoch_ = EPOCH) {
		epoch = epoch_;
		num = num_;
	}
	void set_GA(long* Wi_, long* Pi_, int N_, int W_);	// �ʱ�ȭ ���� �Լ�
	long generate();	// ���� ���� �� �ִ밪 ��ȯ
};

void GA::set_GA(long* Wi_, long* Pi_, int N_, int W_) {
	// N, W �޾ƿ�
	N = N_;
	W = W_;

	// i��° �������� ���� �ʱ�ȭ
	Wi = new long[N + 1];
	for (int i = 0; i <= N; i++)
		Wi[i] = 0;

	// i��° �������� ��ġ �ʱ�ȭ
	Pi = new long[N + 1];
	for (int i = 0; i <= N; i++)
		Pi[i] = 0;

	// Wi, Pi �޾ƿ�
	for (int i = 1; i <= N; i++) {
		Wi[i] = Wi_[i];
		Pi[i] = Pi_[i];
	}

	// ������ random �ʱ�ȭ
	make_gene();

	// best ������ �ʱ�ȭ
	best_gene.initialize(N);

	// ù ��° ���
	calculate_gene();
}

// �� �����ڸ� random bit�� ����� ������ ���Ϳ� ����
void GA::make_gene() {
	for (int i = 0; i < num; i++) {
		Gene temp;
		temp.initialize(N);
		gene.push_back(temp);
	}
}

long GA::generate() {
	// select, cross over, mutate ����
	for (int kk = 1; kk < epoch; kk++) {
		// ���� ���� ����
		random_device rd;
		mt19937_64 rng(rd());
		uniform_int_distribution<__int64> dist(0, num - 1);
		uniform_int_distribution<__int64> dist2(1, 100);
		uniform_int_distribution<__int64> dist3(0, N - 1);

		vector<Gene> temp_gene;	// ���� ������ ���� ����
		for (int i = 0; i < num; i++)
			temp_gene.push_back(gene[i]);

		// ������ �θ� ������ index
		int index_dad = 0;
		int index_mom = 0;

		for (int i = 0; i < num; i++) {
			// tournament select
			// a, b ���� �ΰ� ���� ( 0 ~ num-1 )
			int a = dist(rng);
			int b = dist(rng);

			// select dad
			if (i % 2 == 0) {
				// �ش� ������ �� fitness�� �����ɷ� ����
				if (gene[a] < gene[b])
					index_dad = b;
				else
					index_dad = a;
			}
			// select mom
			else {
				// �ش� ������ �� fitness�� �����ɷ� ����
				if (gene[a] < gene[b])
					index_mom = b;
				else
					index_mom = a;
			}

			// �θ� �Ѵ� �� ��츸 �ش�
			if (i % 2 != 0) {
				// cross over
				if (dist2(rng) <= CROSS) {
					// �� �� �����ڰ� ���� ���� ���谡 �ǹ̰� ����.
					// �ϳ��� �״�� ����Ǹ�, ������ �ϳ��� random �����Ѵ�. (������ ���ؼ�)
					if (temp_gene[index_dad] == temp_gene[index_mom]) {
						// random ������ ����
						Gene temp;
						temp.initialize(N);
						for (int j = 0; j < N; j++) {
							gene[i - 1].bit[j] = temp_gene[index_dad].bit[j];	// �ϳ��� �״�� ����
							gene[i].bit[j] = temp.bit[j];	// �ϳ��� random ����
						}
					}
					// �� �����ڰ� �ٸ� ���
					else {
						// uniform cross over
						for (int j = 0; j < N; j++) {
							// 1 ~ 100 ���� ����
							int ran = dist2(rng);
							// 1/2 �� Ȯ���� �ش� ��Ʈ ����
							if (ran > 50) {
								gene[i - 1].bit[j] = temp_gene[index_dad].bit[j];
								gene[i].bit[j] = temp_gene[index_mom].bit[j];
							}
							// ������ ������ Ȯ���δ� ���� ���� ����
							else {
								gene[i - 1].bit[j] = temp_gene[index_mom].bit[j];
								gene[i].bit[j] = temp_gene[index_dad].bit[j];
							}
						}
					}
				}
				// Cross over ���� �ʴ� ��� �״�� ����
				else {
					for (int j = 0; j < N; j++) {
						gene[i - 1].bit[j] = temp_gene[index_dad].bit[j];
						gene[i].bit[j] = temp_gene[index_mom].bit[j];
					}
				}
				// insert mutation
				// first child
				if (dist2(rng) <= MUTATE) {
					// �ΰ��� ��Ʈ�� ������ �ڸ� ����
					int a = dist3(rng);
					int b = dist3(rng);
					int temp = gene[i - 1].bit[a];
					gene[i - 1].bit[a] = gene[i - 1].bit[b];
					gene[i - 1].bit[b] = temp;
				}
				// second child
				if (dist2(rng) <= MUTATE) {
					// �ΰ��� ��Ʈ�� ������ �ڸ� ����
					int a = dist3(rng);
					int b = dist3(rng);
					int temp = gene[i].bit[a];
					gene[i].bit[a] = gene[i].bit[b];
					gene[i].bit[b] = temp;
				}
			}
		}
		// ����, �������̰� �������Ƿ� �ٽ� ���
		calculate_gene();
	}
	return best_price;
}

// ���յ� ����ϴ� �Լ�
void GA::calculate_gene() {
	for (int i = 0; i < num; i++) {
		long sum_P = 0;	// ��ġ �� �����ϴ� ����
		long sum_W = 0;	// ���� �� �����ϴ� ����

		for (int j = 0; j < N; j++) {
			// �ش� ��Ʈ�� 1�� ��� ����
			if (gene[i].bit[j]) {
				sum_P += Pi[j + 1];
				sum_W += Wi[j + 1];
			}
		}
		gene[i].fit = sum_P;	// ���յ��� ��ġ�� ��
		// ���Ը� �ʰ��� ���
		if (sum_W > W)
			gene[i].fit -= WEIGHT * (sum_W - W);	// �ʰ��� ���Ը�ŭ ����ġ�� ũ���־� ���յ��� ���ҽ�Ŵ
		// ���Ը� �ʰ����� ���� ���
		else {
			// �ִ밪���� ��ġ�� ���� �� ū���
			if (best_price < sum_P) {
				best_price = sum_P;	// �ִ밪�� ��ü���ش�.
				for (int j = 0; j < N; j++)
					best_gene.bit[j] = gene[i].bit[j];
			}
		}
	}
}

#endif