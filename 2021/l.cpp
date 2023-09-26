#include <bits/stdc++.h>
// #include <x86intrin.h>
using namespace std;
using namespace numbers;



int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	int nr, nc;
	cin >> nr >> nc;
	swap(nr, nc);
	vector<string> a(nr);
	copy_n(istream_iterator<string>(cin), nr, a.begin());
	vector<vector<int>> res(nr, vector<int>(nc, -1));
	vector<vector<array<int, 2>>> g;
	{
		array<vector<array<int, 2>>, 2> init;
		for(auto x = 0; x < nr; ++ x){
			for(auto y = 0; y < nc; ++ y){
				init[a[x][y] == 'X'].push_back({x, y});
			}
		}
		for(auto t = 0; t < 2; ++ t){
			if((int)init[t].size() == 1){
				auto [x, y] = init[t][0];
				res[x][y] = 0;
			}
			else if(!init[t].empty()){
				g.push_back(init[t]);
			}
		}
	}
	const vector<pair<int, int>> dr4{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
	auto next = [&](array<int, 2> p, int di)->array<int, 2>{
		return {p[0] + dr4[di].first, p[1] + dr4[di].second};
	};
	vector<vector<int>> vis(500, vector<int>(500));
	vis[250][250] = true;
	auto valid = [&](array<int, 2> p)->bool{
		return !vis[p[0] + 250][p[1] + 250];
	};
	auto set_vis = [&](array<int, 2> p)->void{
		vis[p[0] + 250][p[1] + 250] = true;
	};
	auto get = [&](array<int, 2> p)->char{
		return 0 <= min(p[0], p[1]) && p[0] < nr && p[1] < nc ? a[p[0]][p[1]] : '.';
	};
	for(auto x = 0, y = 0, di = 0, step = 1; !g.empty(); ++ step){
		x += dr4[di].first, y += dr4[di].second;
		set_vis({x, y});
		{
			static vector<vector<array<int, 2>>> g_next;
			g_next.clear();
			for(auto c: g){
				static array<vector<array<int, 2>>, 2> next;
				next[0].clear();
				next[1].clear();
				for(auto [xc, yc]: c){
					next[get({x + xc, y + yc}) == 'X'].push_back({xc, yc});
				}
				for(auto t = 0; t < 2; ++ t){
					if((int)next[t].size() == 1){
						auto [x, y] = next[t][0];
						res[x][y] = step;
					}
					else if(!next[t].empty()){
						g_next.push_back(next[t]);
					}
				}
			}
			swap(g, g_next);
		}
		if(valid(next({x, y}, di + 1 & 3))){
			di = di + 1 & 3;
		}
	}
	int sum = 0, opt = 0;
	vector<array<int, 2>> opts;
	for(auto x = nr - 1; x >= 0; -- x){
		for(auto y = 0; y < nc; ++ y){
			sum += res[x][y];
			if(opt < res[x][y]){
				opt = res[x][y];
				opts = {{y + 1, nr - x}};
			}
			else if(opt == res[x][y]){
				opts.push_back({y + 1, nr - x});
			}
		}
	}
	cout << fixed << setprecision(15);
	cout << sum * 1.0 / nr / nc << "\n" << opt << "\n";
	for(auto [x, y]: opts){
		cout << "(" << x << "," << y << ") ";
	}
	cout << "\n";
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////