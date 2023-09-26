#include <bits/stdc++.h>
// #include <x86intrin.h>
using namespace std;
using namespace numbers;



int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	cout << fixed << setprecision(15);
	int np, nc;
	cin >> np >> nc;
	vector<int> cnt(nc);
	copy_n(istream_iterator<int>(cin), nc, cnt.begin());
	vector<int> state(np + nc - 1);
	ranges::fill(state | ranges::views::drop(np), 1);
	auto pick = [&](int n, int k)->double{
		assert(0 <= k && k <= n);
		double res = 1;
		for(auto i = 1; i <= k; ++ i){
			res *= n - i + 1;
		}
		return res;
	};
	auto choose = [&](int n, int k)->double{
		double res = pick(n, k);
		for(auto i = 1; i <= k; ++ i){
			res /= i;
		}
		return res;
	};
	array<double, 2> res{};
	do{
		vector<int> hand(nc);
		for(auto c = 0, l = 0; c < nc; ++ c){
			int r = l;
			while(r < (int)state.size() && !state[r]){
				++ r;
			}
			hand[c] = r - l;
			l = r + 1;
		}
		double left = 0, right = 1, fact = 1;
		for(auto x = 1; x < np; ++ x){
			fact *= x;
		}
		for(auto c = 0; c < nc; ++ c){
			if(cnt[c] < hand[c]){
				goto NEXT;
			}
			right *= choose(cnt[c], hand[c]);
		}
		for(auto c = 0; c < nc; ++ c){
			if(!hand[c]){
				continue;
			}
			double left_cur = fact;
			for(auto d = 0; d < nc; ++ d){
				left_cur *= choose(cnt[d], hand[d] - (c == d));
			}
			left += left_cur;
		}
		res[0] += min(left, right);
		res[1] += right;
		NEXT:;
	}while(ranges::next_permutation(state).found);
	cout << res[0] / res[1] << "\n";
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////