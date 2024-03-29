#include <bits/stdc++.h>
// #include <x86intrin.h>
using namespace std;
using namespace numbers;

template<bool HAS_QUERY, bool HAS_UPDATE, class T, class U, class F1, class F2, class F3>
struct segment_tree_base{
#define ifQ if constexpr(HAS_QUERY)
#define ifU if constexpr(HAS_UPDATE)
	int n, size, log;
	vector<T> data;
	vector<U> data_action;
	F1 TT; // monoid operation (always adjacent)
	T T_id; // monoid identity
	F2 UU; // monoid operation (superset, subset)
	U U_id; // monoid identity
	F3 UT; // action of U on T (superset, subset)
	// O(n)
	segment_tree_base(F1 TT, T T_id, F2 UU, U U_id, F3 UT): TT(TT), T_id(T_id), UU(UU), U_id(U_id), UT(UT){ }
	segment_tree_base &operator=(const segment_tree_base &seg){
		n = seg.n;
		size = seg.size;
		log = seg.log;
		data = seg.data;
		data_action = seg.data_action;
	}
	// O(n)
	void build(int n){
		this->n = n;
		size = 1;
		while(size < n) size <<= 1;
		log = __lg(size);
		ifQ data.assign(size << 1, T_id);
		ifU data_action.assign(HAS_QUERY ? size : size << 1, U_id);
	}
	// O(n)
	void build(int n, T x){
		static_assert(HAS_QUERY);
		this->n = n;
		size = 1;
		while(size < n) size <<= 1;
		log = __lg(size);
		data.assign(size << 1, T_id);
		fill(data.begin() + size, data.begin() + size + n, x);
		for(auto i = size - 1; i >= 1; -- i) refresh(i);
		ifU data_action.assign(size, U_id);
	}
	// O(n)
	void build(const vector<T> &a){
		static_assert(HAS_QUERY);
		n = (int)a.size();
		size = 1;
		while(size < n) size <<= 1;
		log = __lg(size);
		data.assign(size << 1, T_id);
		copy(a.begin(), a.end(), data.begin() + size);
		for(auto i = size - 1; i >= 1; -- i) refresh(i);
		ifU data_action.assign(size, U_id);
	}
	// O(n)
	void build_action(int n){
		static_assert(!HAS_QUERY && HAS_UPDATE);
		build(n);
	}
	// O(n)
	void build_action(int n, U f){
		static_assert(!HAS_QUERY && HAS_UPDATE);
		this->n = n;
		size = 1;
		while(size < n) size <<= 1;
		log = __lg(size);
		data_action.assign(size << 1, U_id);
		fill(data_action.begin() + size, data_action.begin() + size + n, f);
	}
	// O(n)
	void build_action(const vector<U> &a){
		static_assert(!HAS_QUERY && HAS_UPDATE);
		n = (int)a.size();
		size = 1;
		while(size < n) size <<= 1;
		log = __lg(size);
		data_action.assign(size << 1, U_id);
		copy(a.begin(), a.end(), data_action.begin() + size);
	}
	// O(1)
	void refresh(int i){
		static_assert(HAS_QUERY);
		data[i] = TT(data[i << 1], data[i << 1 | 1]);
	}
	// O(1)
	void apply(int i, U f){
		static_assert(HAS_UPDATE);
		ifQ data[i] = UT(f, data[i]);
		if(!HAS_QUERY || i < size) data_action[i] = UU(f, data_action[i]);
	}
	// O(1)
	void push(int i){
		static_assert(HAS_UPDATE);
		apply(i << 1, data_action[i]), apply(i << 1 | 1, data_action[i]);
		data_action[i] = U_id;
	}
	// O(log(n)) if HAS_UPDATE, O(1) otherwise.
	T query(int p){
		static_assert(HAS_QUERY);
		assert(0 <= p && p < n);
		p += size;
		ifU for(auto i = log; i >= 1; -- i) push(p >> i);
		return data[p];
	}
	// O(log(n))
	U query_action(int p){
		static_assert(!HAS_QUERY && HAS_UPDATE);
		assert(0 <= p && p < n);
		p += size;
		ifU for(auto i = log; i >= 1; -- i) push(p >> i);
		return data_action[p];
	}
	// O(log(n))
	T query(int l, int r){
		static_assert(HAS_QUERY);
		assert(0 <= l && l <= r && r <= n);
		if(l == r) return T_id;
		l += size, r += size;
		ifU for(auto i = log; i >= 1; -- i){
			if(l >> i << i != l) push(l >> i);
			if(r >> i << i != r) push(r - 1 >> i);
		}
		T res_left = T_id, res_right = T_id;
		for(; l < r; l >>= 1, r >>= 1){
			if(l & 1) res_left = TT(res_left, data[l ++]);
			if(r & 1) res_right = TT(data[-- r], res_right);
		}
		return TT(res_left, res_right);
	}
	// O(1)
	T query_all() const{
		static_assert(HAS_QUERY);
		return data[1];
	}
	// O(n)
	vector<T> to_array(){
		static_assert(HAS_QUERY);
		ifU for(auto i = 1; i < size; ++ i) push(i);
		return vector<T>(data.begin() + size, data.begin() + size + n);
	}
	// O(n)
	vector<U> to_array_of_updates(){
		static_assert(!HAS_QUERY && HAS_UPDATE);
		for(auto i = 1; i < size; ++ i) push(i);
		return vector<U>(data_action.begin() + size, data_action.begin() + size + n);
	}
	// O(log(n))
	void set(int p, T x){
		static_assert(HAS_QUERY);
		assert(0 <= p && p < n);
		p += size;
		ifU for(auto i = log; i >= 1; -- i) push(p >> i);
		data[p] = x;
		for(auto i = 1; i <= log; ++ i) refresh(p >> i);
	}
	// O(log(n))
	void set_action(int p, U f){
		static_assert(!HAS_QUERY && HAS_UPDATE);
		assert(0 <= p && p < n);
		p += size;
		for(auto i = log; i >= 1; -- i) push(p >> i);
		data_action[p] = f;
	}
	// O(log(n))
	void update(int p, U f){
		static_assert(HAS_UPDATE);
		assert(0 <= p && p < n);
		p += size;
		for(auto i = log; i >= 1; -- i) push(p >> i);
		ifQ{
			data[p] = UT(f, data[p]);
			for(auto i = 1; i <= log; ++ i) refresh(p >> i);
		}
		else data_action[p] = UU(f, data_action[p]);
	}
	// O(log(n))
	void update(int l, int r, U f){
		static_assert(HAS_UPDATE);
		assert(0 <= l && l <= r && r <= n);
		if(l == r) return;
		l += size, r += size;
		for(auto i = log; i >= 1; -- i){
			if(l >> i << i != l) push(l >> i);
			if(r >> i << i != r) push(r - 1 >> i);
		}
		int l2 = l, r2 = r;
		for(; l < r; l >>= 1, r >>= 1){
			if(l & 1) apply(l ++, f);
			if(r & 1) apply(-- r, f);
		}
		l = l2, r = r2;
		ifQ for(auto i = 1; i <= log; ++ i){
			if(l >> i << i != l) refresh(l >> i);
			if(r >> i << i != r) refresh(r - 1 >> i);
		}
	}
	// pred(sum[l, r)) is T, T, ..., T, F, F, ..., F
	// Returns max r with T
	// O(log(n))
	int max_pref(int l, auto pred){
		static_assert(HAS_QUERY);
		assert(0 <= l && l <= n && pred(T_id));
		if(l == n) return n;
		l += size;
		ifU for(auto i = log; i >= 1; -- i) push(l >> i);
		T sum = T_id;
		do{
			while(~l & 1) l >>= 1;
			if(!pred(TT(sum, data[l]))){
				while(l < size){
					ifU push(l);
					l = l << 1;
					if(pred(TT(sum, data[l]))) sum = TT(sum, data[l ++]);
				}
				return l - size;
			}
			sum = TT(sum, data[l]);
			++ l;
		}while((l & -l) != l);
		return n;
	}
	// pred(sum[l, r)) is F, F, ..., F, T, T, ..., T
	// Returns min l with T
	// O(log(n))
	int min_suff(int r, auto pred){
		static_assert(HAS_QUERY);
		assert(0 <= r && r <= n && pred(T_id));
		if(r == 0) return 0;
		r += size;
		ifU for(auto i = log; i >= 1; -- i) push(r - 1 >> i);
		T sum = T_id;
		do{
			-- r;
			while(r > 1 && r & 1) r >>= 1;
			if(!pred(TT(data[r], sum))){
				while(r < size){
					ifU push(r);
					r = r << 1 | 1;
					if(pred(TT(data[r], sum))) sum = TT(data[r --], sum);
				}
				return r + 1 - size;
			}
			sum = TT(data[r], sum);
		}while((r & -r) != r);
		return 0;
	}
	template<class output_stream>
	friend output_stream &operator<<(output_stream &out, segment_tree_base<HAS_QUERY, HAS_UPDATE, T, U, F1, F2, F3> seg){
		out << "{";
		for(auto i = 0; i < seg.n; ++ i){
			ifQ out << seg.query(i);
			else out << seg.query_action(i);
			if(i != seg.n - 1) out << ", ";
		}
		return out << '}';
	}
};

// Supports query
template<class T, class F>
auto make_Q_segment_tree(F TT, T T_id){
	using U = int;
	auto _UU = [&](U, U)->U{ return U{}; };
	auto _UT = [&](U, T)->T{ return T{}; };
	return segment_tree_base<true, false, T, U, F, decltype(_UU), decltype(_UT)>(TT, T_id, _UU, U{}, _UT);
}
// Supports update
template<class U, class F>
auto make_U_segment_tree(F UU, U U_id){
	using T = int;
	auto _TT = [&](T, T)->T{ return T{}; };
	auto _UT = [&](U, T)->T{ return T{}; };
	return segment_tree_base<false, true, T, U, decltype(_TT), F, decltype(_UT)>(_TT, T{}, UU, U_id, _UT);
}
// Supports query and update
template<class T, class U, class F1, class F2, class F3>
auto make_QU_segment_tree(F1 TT, T T_id, F2 UU, U U_id, F3 UT){
	return segment_tree_base<true, true, T, U, F1, F2, F3>(TT, T_id, UU, U_id, UT);
}

int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	int n, qn, obj;
	cin >> n >> qn >> obj, -- obj;
	using T = array<int, 2>;
	auto seg = make_QU_segment_tree([&](T x, T y){ return T{max(x[0], y[0]), x[1] + y[1]}; }, T{}, plus<>(), 0, [&](int f, T x){ return T{x[0] + f, x[1]}; });
	vector<array<int, 2>> init(n);
	for(auto i = 0; i < n; ++ i){
		init[i] = {min({abs(obj - i), abs(obj - i + n), abs(obj - i - n)}), 1};
	}
	seg.build(init);
	vector<array<int, 2>> q(qn);
	for(auto &[d, i]: q){
		cin >> d >> i, -- i;
	}
	auto prev = [&](int i)->int{
		return i ? i - 1 : n - 1;
	};
	auto next = [&](int i)->int{
		return i < n - 1 ? i + 1 : 0;
	};
	for(ranges::sort(q | ranges::views::reverse); auto [_, i]: q){
		int j = next(i);
		int x = seg.query(i)[0];
		int y = seg.query(j)[0];
		swap(x, y);
		x = min(x, seg.query(prev(i))[0] + 1);
		y = min(y, seg.query(next(j))[0] + 1);
		seg.set(i, {x, 1}), seg.set(j, {y, 1});
		{
			int jj = seg.max_pref(j + 1, [&](T sum){ return sum[1] == 0 || sum[0] > sum[1] + y; });
			seg.update(j + 1, jj, -1);
			jj = seg.max_pref(0, [&](T sum){ return sum[1] == 0 || sum[0] > sum[1] + y + n - 1 - j; });
			seg.update(0, jj, -1);
		}
		{
			int ii = seg.min_suff(i, [&](T sum){ return sum[1] == 0 || sum[0] > sum[1] + x; });
			seg.update(ii, i, -1);
			ii = seg.min_suff(n, [&](T sum){ return sum[1] == 0 || sum[0] > sum[1] + x + i; });
			seg.update(ii, n, -1);
		}
	}
	for(auto [x, len]: seg.to_array()){
		cout << x << " ";
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