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
	int n, qn;
	cin >> n >> qn;
	const int mx = 26;
	vector<array<int, mx>> next{{}};
	vector<int> weight(1);
	auto new_node = [&]()->int{
		int u = (int)next.size();
		weight.push_back(0);
		next.push_back({});
		return u;
	};
	vector<int> id(n + 1);
	for(auto u = 1; u <= n; ++ u){
		char c;
		int p;
		cin >> c >> p;
		if(!next[id[p]][c - 'A']){
			next[id[p]][c - 'A'] = new_node();
		}
		id[u] = next[id[p]][c - 'A'];
		++ weight[id[u]];
	}
	vector<int> q(qn);
	for(auto qi = 0; qi < qn; ++ qi){
		string s;
		cin >> s;
		int u = 0;
		for(auto c: s | ranges::views::reverse){
			if(!next[u][c - 'A']){
				next[u][c - 'A'] = new_node();
			}
			u = next[u][c - 'A'];
		}
		q[qi] = u;
	}
	n = (int)next.size();
	vector<int> link(n);
	vector<vector<int>> rlink(n);
	{
		deque<int> dq{0};
		while(!dq.empty()){
			int u = dq.front();
			dq.pop_front();
			for(auto c = 0; c < mx; ++ c){
				if(!next[u][c]){
					continue;
				}
				int v = next[u][c];
				dq.push_back(v);
				if(u){
					link[v] = next[link[u]][c];
				}
				rlink[link[v]].push_back(v);
			}
			if(u){
				for(auto c = 0; c < mx; ++ c){
					if(!next[u][c]){
						next[u][c] = next[link[u]][c];
					}
				}
			}
		}
	}
	auto dfs = [&](auto self, int u)->void{
		for(auto v: rlink[u]){
			self(self, v);
			weight[u] += weight[v];
		}
	};
	dfs(dfs, 0);
	for(auto qi = 0; qi < qn; ++ qi){
		cout << weight[q[qi]] << "\n";
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