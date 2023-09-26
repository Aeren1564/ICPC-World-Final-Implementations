#include <bits/stdc++.h>
// #include <x86intrin.h>
using namespace std;
using namespace numbers;

struct fast_fourier_transform_wrapper{
	using CD = complex<double>;
	using CLD = complex<long double>;
	// i \in [2^k, 2^{k+1}) holds w_{2^k+1}^{i-2^k}
	static vector<CD> root;
	static vector<CLD> root_ld;
	static void adjust_root(int n){
		if(root.empty()) root = {1, 1}, root_ld = {1, 1};
		for(auto k = (int)root.size(); k < n; k <<= 1){
			root.resize(n), root_ld.resize(n);
			auto theta = polar(1.0L, acos(-1.0L) / k);
			for(auto i = k; i < k << 1; ++ i) root[i] = root_ld[i] = i & 1 ? root_ld[i >> 1] * theta : root_ld[i >> 1];
		}
	}
	// O(n * log(n))
	static void transform(vector<CD> &p, bool invert = false){
		int n = (int)p.size();
		assert(n && __builtin_popcount(n) == 1);
		for(auto i = 1, j = 0; i < n; ++ i){
			int bit = n >> 1;
			for(; j & bit; bit >>= 1) j ^= bit;
			j ^= bit;
			if(i < j) swap(p[i], p[j]);
		}
		adjust_root(n);
		for(auto len = 1; len < n; len <<= 1) for(auto i = 0; i < n; i += len << 1) for(auto j = 0; j < len; ++ j){
			auto x = (double *)&root[j + len], y = (double *)&p[i + j + len];
			CD z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);
			p[len + i + j] = p[i + j] - z, p[i + j] += z;
		}
		if(invert){
			reverse(p.begin() + 1, p.end());
			auto inv_n = 1.0l / n;
			for(auto &x: p) x *= inv_n;
		}
	}
	static vector<CD> buffer1, buffer2;
	// O(n * m)
	template<class T>
	static vector<T> convolute_naive(const vector<T> &p, const vector<T> &q){
		vector<T> res(max((int)p.size() + (int)q.size() - 1, 0));
		for(auto i = 0; i < (int)p.size(); ++ i) for(auto j = 0; j < (int)q.size(); ++ j) res[i + j] += p[i] * q[j];
		return res;
	}
	// Safe for sum(p[i]^2 + q[i]^2) lg2(n) < 9e14
	// O(n * log(n))
	template<class T>
	static vector<T> convolute(const vector<T> &p, const vector<T> &q){
		if(min(p.size(), q.size()) < 60) return convolute_naive(p, q);
		int n = 1 << __lg((int)p.size() + (int)q.size() - 1) + 1;
		buffer1.assign(n, 0);
		for(auto i = 0; i < (int)p.size(); ++ i) buffer1[i].real(p[i]);
		for(auto i = 0; i < (int)q.size(); ++ i) buffer1[i].imag(q[i]);
		transform(buffer1);
		for(auto &x: buffer1) x *= x;
		buffer2.assign(n, 0);
		for(auto i = 0; i < n; ++ i) buffer2[i] = buffer1[i] - conj(buffer1[-i & n - 1]);
		transform(buffer2, true);
		vector<T> res((int)p.size() + (int)q.size() - 1);
		for(auto i = 0; i < (int)res.size(); ++ i) res[i] = llround(buffer2[i].imag() / 4);
		return res;
	}
	// O(n * log(n))
	static vector<CD> convolute_complex(const vector<CD> &p, const vector<CD> &q){
		if(min(p.size(), q.size()) < 60) return convolute_naive(p, q);
		int n = 1 << __lg((int)p.size() + (int)q.size() - 1) + 1;
		buffer1 = p, buffer2 = q;
		buffer1.resize(n), buffer2.resize(n);
		transform(buffer1), transform(buffer2);
		for(auto i = 0; i < n; ++ i) buffer1[i] *= buffer2[i];
		transform(buffer1, true);
		return {buffer1.begin(), buffer1.begin() + ((int)p.size() + (int)q.size() - 1)};
	}
	// Safe for 64-bit integer range
	// O(n * log(n))
	template<class T>
	static vector<T> convolute_splitting(const vector<T> &p, const vector<T> &q){
		if(min(p.size(), q.size()) < 80) return convolute_naive(p, q);
		int n = 1 << __lg((int)p.size() + (int)q.size() - 1) + 1;
		const int cut = 32000;
		buffer1.assign(n, 0);
		for(auto i = 0; i < (int)p.size(); ++ i) buffer1[i] = {(int)p[i] / cut, (int)p[i] % cut};
		transform(buffer1);
		buffer2.assign(n, 0);
		for(auto i = 0; i < (int)q.size(); ++ i) buffer2[i] = {(int)q[i] / cut, (int)q[i] % cut};
		transform(buffer2);
		for(auto i = 0; i <= n >> 1; ++ i){
			int j = -i & n - 1;
			if(i == j){
				tie(buffer1[i], buffer2[i]) = pair<CD, CD>{
					(buffer1[i] + conj(buffer1[i])) * buffer2[i] / 2.0,
					(buffer1[i] - conj(buffer1[i])) * buffer2[i] / 2i
				};
			}
			else{
				tie(buffer1[i], buffer2[i], buffer1[j], buffer2[j]) = tuple<CD, CD, CD, CD>{
					(buffer1[i] + conj(buffer1[j])) * buffer2[i] / 2.0,
					(buffer1[i] - conj(buffer1[j])) * buffer2[i] / 2i,
					(buffer1[j] + conj(buffer1[i])) * buffer2[j] / 2.0,
					(buffer1[j] - conj(buffer1[i])) * buffer2[j] / 2i
				};
			}
		}
		transform(buffer1, true);
		transform(buffer2, true);
		vector<T> res((int)p.size() + (int)q.size() - 1);
		for(auto i = 0; i < (int)res.size(); ++ i) res[i] = ((T)llround(buffer1[i].real()) * cut + (T)(llround(buffer1[i].imag()) + llround(buffer2[i].real()))) * cut + (T)llround(buffer2[i].imag());
		return res;
	}
	// Safe for 64-bit integer range
	// O(n * log(n))
	template<class T>
	static vector<T> convolute_splitting_mod(const vector<T> &p, const vector<T> &q){
		if(min(p.size(), q.size()) < 80) return convolute_naive(p, q);
		vector<int> p2(p.begin(), p.end()), q2(q.begin(), q.end());
		p2 = convolute_splitting(p2, q2);
		return {p2.begin(), p2.end()};
	}
};
vector<complex<double>> fast_fourier_transform_wrapper::root;
vector<complex<long double>> fast_fourier_transform_wrapper::root_ld;
vector<complex<double>> fast_fourier_transform_wrapper::buffer1;
vector<complex<double>> fast_fourier_transform_wrapper::buffer2;
 
using fft = fast_fourier_transform_wrapper;

int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	using C = complex<double>;
	int nr0, nc0;
	cin >> nr0 >> nc0;
	vector motif(nr0, vector<int>(nc0));
	for(auto &c: motif | ranges::views::join){
		cin >> c;
	}
	int nr1, nc1;
	cin >> nr1 >> nc1;
	vector mosaic(nr1, vector<int>(nc1));
	for(auto &c: mosaic | ranges::views::join){
		cin >> c;
	}
	if(nr0 > nr1 || nc0 > nc1){
		cout << "0\n";
		return 0;
	}
	vector<C> pattern(nr0 * nc1), pool(nr1 * nc1);
	int zero_cnt = nr0 * (nc1 - nc0);
	for(auto x = 0; x < nr0; ++ x){
		for(auto y = 0; y < nc0; ++ y){
			if(motif[x][y]){
				pattern[nr0 * nc1 - 1 - nc1 * x - y] = exp(1.0i * (C)pi / (C)100 * (C)motif[x][y]);
			}
			else{
				++ zero_cnt;
			}
		}
	}
	for(auto x = 0; x < nr1; ++ x){
		for(auto y = 0; y < nc1; ++ y){
			pool[nc1 * x + y] = exp(1.0i * (C)-pi / (C)100 * (C)mosaic[x][y]);
		}
	}
	auto product = fft::convolute_complex(pattern, pool);
	int obj = nr0 * nc1 - zero_cnt;
	assert(obj >= 0);
	vector<array<int, 2>> res;
	for(auto x = 0; x + nr0 <= nr1; ++ x){
		for(auto y = 0; y + nc0 <= nc1; ++ y){
			int k = nc1 * x + y;
			if(abs(product[nr0 * nc1 - 1 + k] - (C)obj) <= 1e-5){
				res.push_back({x, y});
			}
		}
	}
	cout << (int)res.size() << "\n";
	for(auto [x, y]: res){
		cout << x + 1 << " " << y + 1 << "\n";
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