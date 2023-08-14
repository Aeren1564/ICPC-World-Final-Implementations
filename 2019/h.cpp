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
	int n, k;
	cin >> n >> k;
	vector<int> next(n);
	for(auto u = 0; u < n; ++ u){
		cin >> next[u], -- next[u];
	}
	vector<vector<int>> jump(n, vector<int>(20));
	for(auto u = 0; u < n; ++ u){
		jump[u][0] = next[u];
	}
	for(auto bit = 1; bit < 20; ++ bit){
		for(auto u = 0; u < n; ++ u){
			jump[u][bit] = jump[jump[u][bit - 1]][bit - 1];
		}
	}
	auto do_jump = [&](int u, int k)->int{
		for(auto bit = 0; bit < 20; ++ bit){
			if(k & 1 << bit){
				u = jump[u][bit];
			}
		}
		return u;
	};
	vector<vector<int>> cycle;
	vector<int> cycle_id(n, -1);
	vector<int> cycle_pos(n, -1);
	vector<int> root(n, -1);
	vector<int> depth(n, -1);
	{ // process graph
		vector<int> was(n, -1);
		for(auto u = 0; u < n; ++ u){
			if(!~was[u]){
				vector<int> stack{u};
				was[u] = u;
				while(!~was[next[stack.back()]]){
					stack.push_back(next[stack.back()]);
					was[stack.back()] = u;
				}
				if(was[next[stack.back()]] == u){
					int cycle_start = ranges::find(stack, next[stack.back()]) - stack.begin();
					int cid = (int)cycle.size();
					cycle.push_back(vector<int>(stack.begin() + cycle_start, stack.end()));
					for(auto i = 0; i < (int)cycle[cid].size(); ++ i){
						cycle_id[cycle[cid][i]] = cid;
						cycle_pos[cycle[cid][i]] = i;
						root[cycle[cid][i]] = cycle[cid][i];
						depth[cycle[cid][i]] = 0;
					}
					for(auto u: stack | ranges::views::take(cycle_start) | ranges::views::reverse){
						root[u] = root[next[u]];
						depth[u] = depth[next[u]] + 1;
					}
				}
				else{
					for(auto u: stack | ranges::views::reverse){
						root[u] = root[next[u]];
						depth[u] = depth[next[u]] + 1;
					}
				}
			}
		}
	}
	vector<vector<int>> abr(n);
	for(auto u = 0; u < n; ++ u){
		if(!~cycle_id[u]){
			abr[next[u]].push_back(u);
		}
	}
	vector<vector<int>> update_cycle((int)cycle.size());
	vector<int> res(n);
	for(auto cid = 0; cid < (int)cycle.size(); ++ cid){
		update_cycle[cid].resize((int)cycle[cid].size() + 1);
	}
	auto do_update_cycle = [&](int cid, int p, int len)->void{
		int csize = (int)cycle[cid].size();
		if(len >= csize){
			++ update_cycle[cid][0];
			-- update_cycle[cid][csize];
			return;
		}
		++ update_cycle[cid][p];
		if(p + len <= csize){
			-- update_cycle[cid][p + len];
		}
		else{
			-- update_cycle[cid][csize];
			++ update_cycle[cid][0];
			-- update_cycle[cid][p + len - csize];
		}
	};
	for(auto u = 0; u < n; ++ u){
		if(~cycle_id[u]){
			do_update_cycle(cycle_id[u], cycle_pos[u], k + 1);
		}
		else{
			++ res[u];
			int u_jump = do_jump(u, k);
			if(~cycle_id[u_jump]){
				int r = root[u];
				do_update_cycle(cycle_id[r], cycle_pos[r], k + 1 - (depth[u] - depth[r]));
			}
			else if(!~cycle_id[next[u_jump]]){
				-- res[next[u_jump]];
			}
		}
	}
	for(auto cid = 0; cid < (int)cycle.size(); ++ cid){
		for(auto i = 0; i < (int)cycle[cid].size(); ++ i){
			update_cycle[cid][i + 1] += update_cycle[cid][i];
			res[cycle[cid][i]] += update_cycle[cid][i];
		}
	}
	auto dfs = [&](auto self, int u)->void{
		for(auto v: abr[u]){
			self(self, v);
			res[u] += res[v];
		}
	};
	for(auto u = 0; u < n; ++ u){
		if(~cycle_id[u]){
			for(auto v: abr[u]){
				dfs(dfs, v);
			}
		}
	}
	ranges::copy(res, ostream_iterator<int>(cout, "\n"));
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////
