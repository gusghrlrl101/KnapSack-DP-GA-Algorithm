# KnapSack-DP-GA-Algorithm
0-1 KnapSack  Solved with DP and GA Algorithm



# Intro

Knapsack Problem을 Genetic Algorithm으로 푸는 방식이다.

기본 개체수는 200, epoch는 100, cross over 확률은 90, mutation 확률은 1 로 설정하였다.

우선 유전자 정보를 가지는 Gene Struct를 선언하였다.

0 또는 1을 값으로 가지는 bit를 vector로 담아서 보관하였고, bit의 크기인 size, 적합도인 fit 변수를 같이 가지고 있다.

initiallize라는 함수는 유전자를 최초로 생성할 때 random한 bit를 형성하여 vector에 넣어주는 작업이다.

연산자 오버로딩 부분은 Gene를 비교할 때에는 fit를 기준으로 비교하기 위함이다.

특히 == 연산자는 모든 bit의 값이 같을 때만 true를 반환한다.

다음으로 GA라는 class를 구현하였다. 

여기에는 개체수인 num, 세대 반복 수 epoch, 사용자로부터 입력받았던 N, W, Wi, Pi가 있다.

그리고 유전자를 저장할 수 있는 vector가있고, 최대값을저장하는 변수 best_price가 있다. 

그리고 최대값일 때 의 유전자를 저장하는 best_gene도 있다.

함수에는 우선 기본 생성자가 있는데, 이 때 사용자는 num과 epoch를 입력할 수 있다.

Set_GA는 기본 정보인 N, W, Wi, Pi를 받아와서 저장한다.

Generate는 세대를 진행하고 최대값을 반환하고, 여기에서 calculate_gene는 각 유전자의 적합도를 계산하고,

make_gene는 유전자를 최초에 생성할 때에 사용되는 함수이다.

Set_GA는 입력받아온 N과 W를 저장하고, Wi와 Pi를 저장하기 위해 메모리를 할당받고 일일히 저장해준다.

그리고 make_gene를 호출하여 유전자들을 random으로 생성해준다. 그리고 best_gene도 같이 초기화 해준다.

Make_gene은 개체 수인 num만큼 각각의 유전자에 대하여 Gene struct내의 함수인 initiallizem를

아이템 수인 N만큼으로 설정하여 random bit를 생성하고 이를 gene라는 vector에 저장해준다.

그리고 calculate_gene을 호출하여 적합도를 계산한다. 

## Generate

Generate는 세대를 반복하는 일을 한다.

Select, cross over, mutate순으로 진행이 되고 이는 epoch만큼 반복된다.

여러 개의 난수 생성 범위를 위해 여러 분포를 만들어 준다.

Temp_gene는 현재 유전자 벡터를 저장해준다.

다음 세대로 갈 때 덮어 쓰여야 하므로 따로 저장해둔 것이다. 

## Select

Select는 tournament select 형식으로 진행 하였다.

A, b 라는 두 난수를 생성하고 둘의 적합도를 비교하여 더 큰 유전자가 선택이 된다.

짝수 번째에는 dad 가 선택이 되고 홀수 번째에는 mom이 선택이 된다.

글고 mom을 선택한 경우는 dad와 mom을 모두 선택한 경우 이므로 이 때에는 cross over를 진행한다.

## Cross over

Cross over는 uniform cross over 방식을 사용하였다.

이 것은 각각의 bit 마다 threshold 값 이상이 나오게 되면 두 bit를 교환하는 것이다.

우선 CROSS 라는 cross over 확률, 여기서는 90으로 설정하였다.

90%의 확률로 교배가 일어난다.

이 때 두 유전자가 같은 경우는 교배하여도 똑같고 그러면 후의 세대로 갈수록 다양성이 떨어지기 때문에,

하나만 저장되고 나머지 하나는 random 으로 형성하는 식으로 하여 다양성을 늘려주었다.

Cross over가 진행이 되면 1 ~ 100 의 난수를 생성하고 이게 50보다 낮으면 교배가 진행이 되고,

아닌 경우는 교배가 진행이 되지 않는다.

그리고 다음 세대로 저장되는 유전자는 i-1, i 번째 이런 식으로 저장하기 때문에 앞에서부터 차근차근 저장되기 된다.

그리고 cross over가 진행되지 않는 경우는 바로 저장하게 된다.

## Mutation

Mutation은 insert mutation 방식을 사용하였고, 이 는 random으로 두 위치를 선택 후 두 개를 서로 바꿔주는 방식이다.

여기서 MUTATE는 mutation 확률로 여기서는 1로 설정하였다. 

1%의 확률로 mutation이 진행되고 두 bit를 랜덤하게 골라 교환한다.

이 는 두 자식 유전자 별개의 확률로 진행된다.

그리고 이 과정들이 끝나면 calculate_gene을 호출하여 적합도를 계산하고,

이 모든 과정이 epoch만큼 실행되면 besr_price를 반환한다.

## Calculate

Calculate_gene는 적합도를 계산하는 함수이다.

Pi와 Wi의 합을 저장하는 sum_P와 sum_W를 선언하고 각각의 bit마다 1인 경우는 더해주고 0인 경우는 더하지 않는다.

그러고 나온 값을 적합도로 설정을 하는데, 이 때 무게를 초과한 경우는 올바르지 않은 정답이므로,

초과된 무게만큼을 가중치를 주어 적합도에서 빼주었다.

여기서는 10으로 설정하였다.

무게를 초과하지 않은 경우에는 best_price보다 가치의 합이 큰 경우 값을 바꿔주고 best_gent에 bit를 저장해준다. 
