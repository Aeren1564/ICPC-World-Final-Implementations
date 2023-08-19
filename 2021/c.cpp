#include <x86intrin.h>
#include <bits/stdc++.h>
using namespace std;
#if __cplusplus > 201703L
#include <ranges>
using namespace numbers;
#endif



int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	int n;
	long long m;
	cin >> n >> m;
	vector<__int128_t> power{0, 1};
	for(auto x = 2; ; ++ x){
		__int128_t pw = 1;
		for(auto rep = n - 1; rep; -- rep){
			pw *= x;
			if(pw > m){
				goto DONE;
			}
		}
		power.push_back(pw * x);
	}
	DONE:;
	for(auto q = 2; q < (int)power.size(); ++ q){
		for(auto p = 1; p < q; ++ p){
			if(m % ((power[q] - power[q - p]) / p) == 0){
				cout << p << " " << q << "\n";
				return 0;
			}
		}
	}
	cout << "impossible\n";
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////
