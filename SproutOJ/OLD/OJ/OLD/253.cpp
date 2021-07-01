#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
int main(){
	vector<int> a, vec;
	int n, x; cin >> n;
	for(int i = 0; i < n; i++){
		cin >> x;
		a.push_back(x);
	}
	vec.push_back(a[0]);
	int ways = 0;
	for(int i = 1;i < a.size();i++){
		if(a[i]>=vec.back()){
			vec.push_back(a[i]);
		}
		else{
			*upper_bound( vec.begin(), vec.end(), a[i]) = a[i];
			if(upper_bound( vec.begin(), vec.end(), a[i]) == vec.end()) ways++;
		}
	}
	cout << vec.size() << '\n' << ways << '\n';
	return 0;
}
/*
�d�ҿ�J1
7
4 2 6 5 1 8 3
�d�ҿ�X1
3
4
1 3 6
�d�ҿ�J2
8
3 2 7 8 2 3 4 5
�d�ҿ�X2
5
1
2 5 6 7 8
�d�ҿ�J3
10
99 105 110 100 121 95 48 51 52 53
�d�ҿ�X3
4
2
1 2 3 5
*/
