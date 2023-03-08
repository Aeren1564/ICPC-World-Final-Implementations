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
	long long h, a, b;
	cin >> n >> h >> a >> b;
	vector<array<int, 2>> pos(n);
	for(auto &[x, y]: pos){
		cin >> x >> y;
	}
	const long long inf = numeric_limits<long long>::max() / 2;
	vector<long long> dp(n, inf);
	dp[0] = a * (h - pos[0][1]);
	const int mx = 1e5;
	for(auto s = 1; s < mx << 1; ++ s){
		int i = n - 1, j = n;
		while(i >= 0 && pos[i][0] << 1 > s){
			-- i;
		}
		while(j >= 1 && pos[j - 1][0] << 1 >= s){
			-- j;
		}
		int low = 0;
		auto update = [&](int i)->void{
			if(0 <= i && i < n){
				auto [x, y] = pos[i];
				if(2 * y + abs(2 * x - s) <= 2 * h){
					low = max(low, 2 * x - s);
				}
				else{
					long long d = 4 * (h - y);
					low = max(low, (int)min(((2LL * x - s) * (2 * x - s) + d * (h - y) + d - 1) / d, 2 * h + 1));
				}
			}
		};
		update(i);
		update(j);
		while(i >= 0 && j < n && pos[j][0] - pos[i][0] <= h << 1){
			if(pos[i][0] + pos[j][0] == s && pos[j][0] - pos[i][0] >= low){
				dp[j] = min(dp[j], dp[i] + b * (pos[j][0] - pos[i][0]) * (pos[j][0] - pos[i][0]) + a * (h - pos[j][1]));
			}
			if(s - pos[i][0] <= pos[j][0]){
				-- i;
				update(i);
			}
			else{
				++ j;
				update(j);
			}
		}
	}
	dp[n - 1] == inf ? cout << "impossible\n" : cout << dp[n - 1] << "\n";
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////