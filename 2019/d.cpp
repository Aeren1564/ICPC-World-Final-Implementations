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
	cin >> n;
	const int mx = 1e6;
	vector<int> sum(mx), minsuff(mx);
	vector<vector<array<int, 2>>> pos(mx);
	for(auto i = 0; i < n; ++ i){
		string s;
		cin >> s;
		int type = s[0] == 'e' ? -1 : 1, x = stoi(s.substr(1)) - 1;
		pos[x].push_back({i, type});
		sum[x] += type;
		minsuff[x] = min(minsuff[x], sum[x]);
	}
	vector<int> cnt(n + 1);
	for(auto x = 0; x < mx; ++ x){
		if(sum[x] || pos[x].empty()){
			continue;
		}
		int cur = 0;
		for(auto i = 0; i < (int)pos[x].size() - 1; ++ i){
			cur += pos[x][i][1];
			if(cur == minsuff[x]){
				++ cnt[pos[x][i][0] + 1];
				-- cnt[pos[x][i + 1][0] + 1];
			}
		}
		if(!minsuff[x]){
			++ cnt[0];
			-- cnt[pos[x].front()[0] + 1];
			++ cnt[pos[x].back()[0] + 1];
			-- cnt[n];
		}
	}
	for(auto i = 0; i < n; ++ i){
		cnt[i + 1] += cnt[i];
	}
	cout << ranges::max_element(cnt) - cnt.begin() + 1 << " " << ranges::max(cnt) << "\n";
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////