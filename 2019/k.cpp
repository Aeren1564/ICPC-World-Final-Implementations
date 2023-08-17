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
	cout << fixed << setprecision(15);
	int cut = 1;
	while(true){
		bool ok = true;
		for(auto x = 1; x <= 100; ++ x){
			for(auto y = x + 1; y <= 100; ++ y){
				int a = x / gcd(x, cut), b = y / gcd(y, cut);
				if(gcd(a, b) != 1 && a % b && b % a){
					ok = false;
					goto DONE;
				}
			}
		}
		DONE:;
		if(ok){
			break;
		}
		++ cut;
	}
	int n;
	cin >> n;
	vector<vector<int>> a(n);
	vector<int> period(n);
	for(auto i = 0; i < n; ++ i){
		int x, r, g;
		cin >> x >> r >> g;
		x %= r + g;
		a[i].resize(r + g);
		for(auto j = r; j < r + g; ++ j){
			a[i][(j - x + r + g) % (r + g)] = 1;
		}
		period[i] = (r + g) / gcd(cut, r + g);
	}
	vector<int> expand_to(n);
	for(auto i = 0; i < n; ++ i){
		for(auto j = 0; j < n; ++ j){
			if(period[j] % period[i] == 0){
				expand_to[i] = max(expand_to[i], period[j]);
			}
		}
	}
	vector<double> res(n + 1);
	for(auto rem = 0; rem < cut; ++ rem){
		static vector<vector<int>> state(101);
		for(auto p = 1; p <= 100; ++ p){
			state[p].assign(p, 1);
		}
		auto compute = [&](int p)->double{
			return accumulate(state[p].begin(), state[p].end(), 0.0L) / p;
		};
		double prod = 1;
		bool is_zero = false;
		for(auto i = 0; i < n; ++ i){
			if(!is_zero){
				static vector<int> recover;
				prod /= compute(expand_to[i]);
				recover = state[expand_to[i]];
				for(auto j = 0; j < expand_to[i]; ++ j){
					state[expand_to[i]][j] &= !a[i][(rem + cut * j) % (int)a[i].size()];
				}
				res[i] += prod * compute(expand_to[i]) / cut;
				state[expand_to[i]] = recover;
				for(auto j = 0; j < expand_to[i]; ++ j){
					state[expand_to[i]][j] &= a[i][(rem + cut * j) % (int)a[i].size()];
				}
				prod *= compute(expand_to[i]);
				if(ranges::all_of(state[expand_to[i]], [&](int x){ return x == 0; })){
					is_zero = true;
				}
			}
		}
		res[n] += prod / cut;
	}
	ranges::copy(res, ostream_iterator<double>(cout, "\n"));
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////
