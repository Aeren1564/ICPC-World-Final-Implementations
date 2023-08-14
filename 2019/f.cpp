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
	auto __solve_geo = [](int n, const vector<array<array<int, 2>, 2>> &a)->vector<array<int, 3>>{
		list<int> order;
		map<int, array<int, 2>> event;
		for(auto i = 0; i < n; ++ i){
			event[a[i][0][0]] = {i, 0};
			event[a[i][1][0]] = {i, 1};
		}
		int sweepline;
		auto cmp = [&](int i, int j)->bool{
			return
				(__int128_t(1) * a[i][0][1] * (a[i][1][0] - sweepline) + __int128_t(1) * a[i][1][1] * (sweepline - a[i][0][0])) * (a[j][1][0] - a[j][0][0]) <
				(__int128_t(1) * a[j][0][1] * (a[j][1][0] - sweepline) + __int128_t(1) * a[j][1][1] * (sweepline - a[j][0][0])) * (a[i][1][0] - a[i][0][0]);
		};
		set<int, decltype(cmp)> state(cmp);
		vector<list<int>::iterator> pos(n);
		for(auto [x, e]: event){
			sweepline = x;
			auto [i, type] = e;
			if(type == 0){
				auto it = state.lower_bound(i);
				if(it == state.end()){
					pos[i] = order.insert(order.end(), i);
				}
				else{
					pos[i] = order.insert(pos[*it], i);
				}
				state.insert(it, i);
			}
			else{
				state.erase(i);
			}
		}
		assert((int)order.size() == n);
		vector<array<int, 3>> res;
		for(auto i: order){
			res.push_back({a[i][0][0], a[i][1][0] + 1, a[i][0][1] < a[i][1][1]});
		}
		return res;
	};
	auto __solve_dp = [](int n, int th_low, int th_high, const vector<array<int, 3>> &order)->int{const int inf = numeric_limits<int>::max() - 3;
		int opt = 0;
		// pos, delta
		map<int, int> left{{-1, -1}, {th_low, -inf}};
		map<int, int> right{{th_high, inf}, {inf, 1}};
		for(auto [l, r, dir]: order){
			if(dir){
				// /
				if(prev(left.end())->first <= l){
					int sum = 0;
					for(auto it = right.upper_bound(l); it->first <= r; it = right.erase(it)){
						sum += it->second;
					}
					if(sum){
						right[r] += sum;
						if(!right[r]){
							right.erase(r);
						}
					}
				}
				else{
					auto it = left.upper_bound(l);
					if(r <= it->first){
						continue;
					}
					++ it->second;
					if(!it->second){
						left.erase(it);
					}
					if(r < right.begin()->first){
						-- left[r];
					}
					else{
						++ opt;
						int sum = -1;
						for(auto it = right.begin(); it->first <= r; it = right.erase(it)){
							sum += it->second;
						}
						if(sum){
							right[r] += sum;
							if(!right[r]){
								right.erase(r);
							}
						}
					}
				}
			}
			else{
				// \                  /
				if(r <= right.begin()->first){
					int sum = 0;
					for(auto it = left.lower_bound(r); l <= prev(it)->first; left.erase(prev(it))){
						sum += prev(it)->second;
					}
					if(sum){
						left[l] += sum;
						if(!left[l]){
							left.erase(l);
						}
					}
				}
				else{
					auto it = right.lower_bound(r);
					if(prev(it)->first <= l){
						// cerr << "Case 5\n";
						continue;
					}
					-- prev(it)->second;
					if(!prev(it)->second){
						right.erase(prev(it));
					}
					if(prev(left.end())->first < l){
						++ right[l];
					}
					else{
						++ opt;
						int sum = 1;
						for(auto it = left.end(); l <= prev(it)->first; left.erase(prev(it))){
							sum += prev(it)->second;
						}
						if(sum){
							left[l] += sum;
							if(!left[l]){
								left.erase(l);
							}
						}
					}
				}
			}
		}
		if(th_low < right.begin()->first && prev(left.end())->first < th_high){
		}
		else if(th_high <= prev(left.end())->first){
			auto it = left.end();
			while(th_high <= prev(it)->first){
				-- it;
				++ opt;
			}
		}
		else{
			auto it = right.begin();
			while(it->first <= th_low){
				++ it;
				++ opt;
			};
		}
		return opt;
	};
	auto __solve = [&]()->void{
		int th_low, th_high, n;
		cin >> th_low >> th_high >> n, th_low <<= 1, th_high = th_high << 1 | 1;
		vector<array<array<int, 2>, 2>> a(n);
		for(auto i = 0; i < n; ++ i){
			for(auto &x: a[i] | ranges::views::join){
				cin >> x, x <<= 1;
			}
			if(a[i][0] > a[i][1]){
				swap(a[i][0], a[i][1]);
			}
		}
		auto order = __solve_geo(n, a);
		auto res = __solve_dp(n, th_low, th_high, order);
		cout << res << "\n";
	};
	__solve();
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////