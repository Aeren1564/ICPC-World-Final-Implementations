#include <x86intrin.h>
#include <bits/stdc++.h>
using namespace std;
#if __cplusplus > 201703L
#include <ranges>
using namespace numbers;
#endif
template<class T> T &ctmin(T &x){ return x; }
template<class T, class Head, class ...Tail> T &ctmin(T &x, const Head &h, const Tail &... t){ return ctmin(x = min<T>(x, h), t...); }
template<class T> T &ctmax(T &x){ return x; }
template<class T, class Head, class ...Tail> T &ctmax(T &x, const Head &h, const Tail &... t){ return ctmax(x = max<T>(x, h), t...); }



int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	int n, qn;
	cin >> n >> qn;
	vector<vector<array<int, 2>>> adj(n);
	long long sum = 0;
	for(auto i = 0; i < n - 1; ++ i){
		int u, v, w;
		cin >> u >> v >> w, -- u, -- v;
		adj[u].push_back({v, w});
		adj[v].push_back({u, w});
		sum += w << 1;
	}
	vector dist(n, vector<long long>(n));
	for(auto s = 0; s < n; ++ s){
		auto dfs = [&](auto self, int u, int _pv)->void{
			for(auto [v, w]: adj[u]){
				if(v == _pv){
					continue;
				}
				dist[s][v] = dist[s][u] + w;
				self(self, v, u);
			}
		};
		dfs(dfs, s, -1);
	}
	vector<int> depth(n), pv(n, -1), pw(n), order, pos(n, -1), end(n, -1);
	vector lca(n, vector<int>(n));
	{
		auto dfs = [&](auto self, int u, int _pv)->void{
			pos[u] = (int)order.size();
			order.push_back(u);
			for(auto [v, w]: adj[u]){
				if(v == _pv){
					continue;
				}
				depth[v] = depth[u] + 1;
				pv[v] = u;
				pw[v] = w;
				self(self, v, u);
			}
			end[u] = (int)order.size();
		};
		dfs(dfs, 0, -1);
		for(auto u = 0; u < n; ++ u){
			for(auto [v, w]: adj[u]){
				if(v == pv[u]){
					continue;
				}
				for(auto l = pos[u]; l < pos[v]; ++ l){
					for(auto r = pos[v]; r < end[v]; ++ r){
						lca[order[l]][order[r]] = lca[order[r]][order[l]] = u;
					}
				}
			}
		}
	}
	for(auto qi = 0; qi < qn; ++ qi){
		int s, k, t;
		cin >> s >> k >> t, -- s, -- k, -- t;
		if(dist[s][t] + dist[t][k] == dist[s][k]){
			cout << "impossible\n";
			continue;
		}
		static vector<int> q(n), was(n, -1);
		static vector<long long> dist2(n);
		int qend = 0;
		{
			int _s = s, _k = k, _t = t;
			long long tot = 0;
			q[0] = s, q[1] = k, q[2] = t;
			qend = 3;
			was[s] = was[k] = was[t] = qi;
			auto push_up = [&](int &u)->void{
				tot += pw[u];
				u = pv[u];
				if(was[u] != qi){
					q[qend ++] = u;
					was[u] = qi;
				}
			};
			while(true){
				int maxd = max(max(depth[s], depth[k]), depth[t]);
				if(depth[s] == maxd){
					push_up(s);
				}
				if(depth[k] == maxd){
					push_up(k);
				}
				if(depth[t] == maxd){
					push_up(t);
				}
				if(s == k || k == t || t == s){
					if(s == k){
						swap(k, t);
					}
					if(depth[s] < depth[k]){
						swap(s, k);
					}
					while(depth[s] > depth[k]){
						push_up(s);
					}
					while(s != k){
						push_up(s);
						push_up(k);
					}
					break;
				}
			}
			for(auto i = 0; i < qend; ++ i){
				dist2[q[i]] = tot;
			}
			s = _s, k = _k, t = _t;
		}
		long long res = numeric_limits<long long>::max();
		for(auto i = 0; i < qend; ++ i){
			int u = q[i];
			ctmin(res, -2 * dist2[u] + dist[t][u]);
			for(auto [v, w]: adj[u]){
				if(was[v] == qi){
					continue;
				}
				was[v] = qi;
				dist2[v] = dist2[u] + w;
				q[qend ++] = v;
			}
		}
		cout << res + sum + dist[s][k] + dist[k][t] << "\n";
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
