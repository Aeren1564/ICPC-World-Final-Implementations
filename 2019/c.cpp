#include <bits/stdc++.h>
using namespace std;
#if __cplusplus > 201703L
#include <ranges>
using namespace numbers;
#endif

template<class F>
struct y_combinator_result{
	F f;
	template<class T> explicit y_combinator_result(T &&f): f(forward<T>(f)){ }
	template<class ...Args> decltype(auto) operator()(Args &&...args){ return f(ref(*this), forward<Args>(args)...); }
};
template<class F>
decltype(auto) y_combinator(F &&f){
	return y_combinator_result<decay_t<F>>(forward<F>(f));
}

int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	const int n = 8;
	vector<vector<int>> id(n, vector<int>(n, -1));
	vector<array<int, 2>> pos;
	for(auto i = 0; i < n; ++ i){
		for(auto j = 0; j < n; ++ j){
			if(i + j & 1){
				id[i][j] = (int)pos.size();
				pos.push_back({i, j});
			}
		}
	}
	auto inrange = [&](int x)->bool{
		return 0 <= x && x < n;
	};
	static const vector<pair<int, int>> dr{{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
	vector<int> bad(300, -1);
	bad['b'] = n - 1;
	bad['w'] = 0;
	char first, second;
	int m;
	cin >> first >> m, second = first ^ 'W' ^ 'B';
	vector<int> type(m);
	vector<vector<array<int, 2>>> move(m);
	for(auto t = 0; t < m; ++ t){
		string s;
		cin >> s;
		if(auto it = ranges::find(s, '-'); it != s.end()){
			type[t] = 0;
			move[t] = {
				pos[stoi(string(s.begin(), it)) - 1],
				pos[stoi(string(it + 1, s.end())) - 1]
			};
		}
		else{
			type[t] = 1;
			static const string delim = "x";
			for(auto word: ranges::views::split(s, delim)){
				int x = 0;
				for(auto c: word){
					x = x * 10 + c - '0';
				}
				move[t].push_back(pos[x - 1]);
			}
		}
	}
	using B = array<string, n>;
	B temp;
	temp.fill(string(n, '-'));
	for(auto i = 0; i < n; ++ i){
		for(auto j = 0; j < n; ++ j){
			if(i + j & 1){
				temp[i][j] = '.';
			}
		}
	}
	auto resptr = y_combinator([&](auto self, B init)->optional<array<B, 2>>{
		B state = init;
		array<array<array<int, 2>, n>, n> mapper;
		for(auto i = 0; i < n; ++ i){
			for(auto j = 0; j < n; ++ j){
				if((i + j) % 2 == 0 || init[i][j] == '.'){
					mapper[i][j] = {-1, -1};
				}
				else{
					mapper[i][j] = {i, j};
				}
			}
		}
		for(auto t = 0; t < m; ++ t){
			char upper = t & 1 ? second : first;
			char eupper = upper ^ 'B' ^ 'W';
			char lower = tolower(upper);
			char elower = lower ^ 'b' ^ 'w';
			if(type[t] == 0){
				auto [i, j] = move[t][0];
				auto [k, l] = move[t][1];
				bool forward = (i < k) == (lower == 'b');
				if(state[i][j] != '.' && tolower(state[i][j]) == elower || state[k][l] != '.'){
					return {};
				}
				if(state[i][j] == '.'){
					assert(init[i][j] == '.');
					init[i][j] = state[i][j] = forward && i != bad[lower] ? lower : upper;
					mapper[i][j] = {i, j};
				}
				else if(state[i][j] == lower && !forward){
					auto [_i, _j] = mapper[i][j];
					init[_i][_j] = state[i][j] = upper;
				}
				state[k][l] = state[i][j];
				mapper[k][l] = mapper[i][j];
				state[i][j] = '.';
				mapper[i][j] = {-1, -1};
				if(bad[lower] == k){
					state[k][l] = upper;
				}
			}
			else{
				assert((int)move[t].size() >= 2);
				auto [i0, j0] = move[t].front();
				auto [i1, j1] = move[t].back();
				if(state[i0][j0] == '.'){
					assert(init[i0][j0] == '.');
					init[i0][j0] = state[i0][j0] = i0 != bad[lower] ? lower : upper;
					mapper[i0][j0] = {i0, j0};
				}
				if(tolower(state[i0][j0]) != lower){
					return {};
				}
				if(state[i0][j0] == lower){
					for(auto k = 1; k < (int)move[t].size(); ++ k){
						auto [pi, pj] = move[t][k - 1];
						auto [ni, nj] = move[t][k];
						if(lower == 'b' && pi > ni || lower == 'w' && pi < ni){
							auto [_i0, _j0] = mapper[i0][j0];
							init[_i0][_j0] = state[i0][j0] = upper;
							break;
						}
					}
				}
				for(auto k = 1; k < (int)move[t].size(); ++ k){
					auto [pi, pj] = move[t][k - 1];
					auto [ni, nj] = move[t][k];
					int i = pi + ni >> 1, j = pj + nj >> 1;
					assert(state[pi][pj] != '.' && tolower(state[pi][pj]) == lower);
					if(state[ni][nj] != '.'){
						return {};
					}
					if(state[i][j] == '.'){
						assert(init[i][j] == '.');
						init[i][j] = state[i][j] = i != bad[elower] ? elower : eupper;
						mapper[i][j] = {i, j};
					}
					if(tolower(state[i][j]) != elower){
						return {};
					}
					state[ni][nj] = state[pi][pj];
					mapper[ni][nj] = mapper[pi][pj];
					state[pi][pj] = state[i][j] = '.';
					mapper[pi][pj] = mapper[i][j] = {-1, -1};
				}
				if(bad[lower] == i1){
					state[i1][j1] = upper;
				}
			}
		}
		// Deal with the forced capture
		state = init;
		for(auto t = 0; t < m; ++ t){
			char upper = t & 1 ? second : first;
			char eupper = upper ^ 'B' ^ 'W';
			char lower = tolower(upper);
			char elower = lower ^ 'b' ^ 'w';
			if(type[t] == 0){
				for(auto i = 0; i < n; ++ i){
					for(auto j = 0; j < n; ++ j){
						if((i + j) % 2 == 0 || state[i][j] == '.' || tolower(state[i][j]) != lower){
							continue;
						}
						for(auto [dx, dy]: dr){
							if(state[i][j] == 'b' && dx == -1 || state[i][j] == 'w' && dx == 1){
								continue;
							}
							int ni = i + dx, nj = j + dy;
							int nni = ni + dx, nnj = nj + dy;
							if(!inrange(nni) || !inrange(nnj) || state[nni][nnj] != '.' || state[ni][nj] == '.' || tolower(state[ni][nj]) != elower){
								continue;
							}
							if(init[nni][nnj] != '.'){
								return {};
							}
							for(auto c: {'b', 'w'}){
								init[nni][nnj] = bad[c] != nni ? c : toupper(c);
								if(auto resptr = self(init)){
									return resptr;
								}
							}
							return {};
						}
					}
				}
				auto [i, j] = move[t][0];
				auto [k, l] = move[t][1];
				assert(state[i][j] != '.' && tolower(state[i][j]) == lower && state[k][l] == '.');
				state[k][l] = state[i][j];
				state[i][j] = '.';
				if(bad[lower] == k){
					state[k][l] = upper;
				}
			}
			else{
				for(auto k = 1; k < (int)move[t].size(); ++ k){
					auto [pi, pj] = move[t][k - 1];
					auto [ni, nj] = move[t][k];
					int i = pi + ni >> 1, j = pj + nj >> 1;
					assert(state[pi][pj] != '.' && tolower(state[pi][pj]) == lower && state[i][j] != '.' && tolower(state[i][j]) == elower && state[ni][nj] == '.');
					state[ni][nj] = state[pi][pj];
					state[pi][pj] = state[i][j] = '.';
				}
				auto [i1, j1] = move[t].back();
				for(auto [dx, dy]: dr){
					if(state[i1][j1] == 'b' && dx == -1 || state[i1][j1] == 'w' && dx == 1){
						continue;
					}
					int ni = i1 + dx, nj = j1 + dy;
					int nni = ni + dx, nnj = nj + dy;
					if(!inrange(nni) || !inrange(nnj) || state[nni][nnj] != '.' || state[ni][nj] == '.' || tolower(state[ni][nj]) != elower){
						continue;
					}
					if(init[nni][nnj] != '.'){
						return {};
					}
					for(auto c: {'b', 'w'}){
						init[nni][nnj] = bad[c] != nni ? c : toupper(c);
						if(auto resptr = self(init)){
							return resptr;
						}
					}
					return {};
				}
				if(bad[lower] == i1){
					state[i1][j1] = upper;
				}
			}
		}
		return array{init, state};
	})(temp);
	assert(resptr);
	auto [init, state] = *resptr;
	for(auto i = 0; i < n; ++ i){
		cout << init[i] << " " << state[i] << "\n";
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