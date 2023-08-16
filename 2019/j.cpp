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
	vector score(n, vector<int>(h));
	vector<long long> sum(n);
	vector temp(n, vector<array<int, 2>>(h));
	for(auto i = 0; i < n; ++ i){
		copy_n(istream_iterator<int>(cin), h, score[i].begin());
		sum[i] = accumulate(score[i].begin(), score[i].end(), 0LL);
		ranges::sort(score[i]);
		for(auto j = 0; j < h; ++ j){
			temp[i][j] = {score[i][j], i};
		}
	}
	for(auto i = 0; i < n; ++ i){
		static vector<array<int, 2>> delta;
		delta.clear();
		for(auto j = 0; j < n; ++ j){
			if(i == j){
				continue;
			}
			static vector<array<int, 2>> event;
			event.clear();
			ranges::merge(temp[i], temp[j], back_inserter(event));
			long long coef = sum[i] - sum[j];
			int slope = 0;
			for(auto t = (int)event.size() - 2; t >= 0; -- t){
				if(event[t + 1][1] == i){
					++ slope;
					coef -= event[t + 1][0];
				}
				else{
					-- slope;
					coef += event[t + 1][0];
				}
				if((coef + 1LL * slope * event[t][0] >= 0) != (coef + 1LL * slope * event[t + 1][0] >= 0)){
					if(coef + 1LL * slope * event[t][0] >= 0){
						assert(slope < 0);
						delta.push_back({coef / -slope + 1, -1});
					}
					else{
						assert(slope > 0);
						delta.push_back({(-coef + slope - 1) / slope, 1});
					}
				}
			}
		}
		ranges::sort(delta);
		int res = n, rank = n;
		for(auto l = 0; l < (int)delta.size(); ){
			int r = l;
			while(r < (int)delta.size() && delta[l][0] == delta[r][0]){
				rank += delta[r][1];
				++ r;
			}
			res = min(res, rank);
			l = r;
		}
		cout << res << "\n";
	}
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////