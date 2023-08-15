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
	int n, h;
	cin >> n >> h;
	vector<vector<int>> a(n, vector<int>(h));
	map<int, vector<int>> event;
	for(auto i = 0; i < n; ++ i){
		for(auto j = 0; j < h; ++ j){
			cin >> a[i][j];
			event[a[i][j]].push_back(i);
		}
		ranges::sort(a[i]);
	}
	vector<int> rank(n, n);
	vector<int> slope(n);
	vector<long long> coef(n);
	for(auto i = 0; i < n; ++ i){
		coef[i] = accumulate(a[i].begin(), a[i].end(), 0LL);
	}
	for(auto [x, e]: event | ranges::views::reverse){
		for(auto i: e){
			++ slope[i];
			coef[i] -= x;
		}
		static vector<long long> score(n);
		for(auto i = 0; i < n; ++ i){
			score[i] = coef[i] + slope[i] * x;
		}
		static vector<int> order(n);
		iota(order.begin(), order.end(), 0);
		ranges::sort(order, [&](int i, int j){ return score[i] < score[j]; });
		for(auto l = 0; l < n; ){
			int r = l;
			while(r < n && score[order[l]] == score[order[r]]){
				++ r;
			}
			for(auto i = l; i < r; ++ i){
				rank[order[i]] = min(rank[order[i]], r);
			}
			l = r;
		}
	}
	ranges::copy(rank, ostream_iterator<int>(cout, "\n"));
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////
