#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <algorithm>
#include <numeric>
using namespace std;

class Solution {
public:
	vector<string> basicCalculatorIV(string expression, const vector<string>& evalvars, const vector<int>& evalints) {
		unordered_map<string, int> m;
		const int n = evalvars.size();
		m.reserve(n);
		for (int i = 0; i < n; ++i) m.emplace(evalvars[i], evalints[i]);
		string s = "";
		const int len = expression.size();
		for (int i = 0; i < len; ++i) {
			if (isalpha(expression[i])) {
				string str = "";
				while (i < len && isalpha(expression[i])) str += expression[i++];
				--i;
				if (m.count(str)) {
					int num = m[str];
					if (num > 0 || s.empty() || s.back() != '-') {
						s += to_string(num);
					}
					else {
						s.back() = '+';
						s += to_string(-num);
					}
				}
				else
					s += str;
			}
			else if (expression[i] != ' ') {
				s += expression[i];
			}
		}
		int i = 0;
		return evaluation(helper(s, i));
	}

	vector<string> helper(const string& s, int& i) {
		vector<string> st;
		char op = '+';
		const int len = s.length();
		int prev_size = 0;
		for (; i < len && op != ')'; ++i) {
			string cur = "";
			if (isalpha(s[i]) || isdigit(s[i]) || (op == '*' && s[i] == '-')) {
				cur += s[i++];
				while (i < len && (isalpha(s[i]) || isdigit(s[i]))) cur += s[i++];
				switch (op) {
				case '+': st.emplace_back("1*" + cur); prev_size = 1;  break;
				case '-': st.emplace_back("-1*" + cur); prev_size = 1; break;
				case '*': for (auto i = st.size() - prev_size; i < st.size(); ++i) st[i] += '*' + cur; break;
				}
			}
			else if (s[i] == '(') {
				auto s_v = helper(s, ++i);
				if (op == '+') {
					for (const auto& str : s_v) st.emplace_back("1*" + str);
					prev_size = s_v.size();
				}
				else if (op == '-') {
					for (const auto& str : s_v) st.emplace_back("-1*" + str);
					prev_size = s_v.size();
				}
				else {
					vector<string> tmp;
					int next_prev = prev_size * s_v.size();
					tmp.reserve(next_prev);
					for (auto i = st.size() - prev_size; i < st.size(); ++i) {
						for (const auto& str : s_v) tmp.emplace_back(st[i] + '*' + str);
					}
					for (int i = 0; i < prev_size; ++i) {
						st.pop_back();
					}
					st.reserve(st.size() + tmp.size());
					for (const auto& str : tmp) {
						st.emplace_back(str);
					}
					prev_size = next_prev;
				}
			}
			if (i < len) op = s[i];
		}
		return st;
	}

	vector<string> evaluation(const vector<string>& s_v) {
		vector<int> nums;
		vector<string> res;
		unordered_map<string, int> m;
		for (const auto& s : s_v) {
			if (IsNumber(s))
				nums.emplace_back(GetNumber(s));
			else
				eval_s(s, m);
		}
		for (const auto& mm : m) {
			if (mm.second != 0) res.emplace_back(to_string(mm.second) + mm.first);
		}
		sort(res.begin(), res.end(), cmp);
		int num_total = accumulate(nums.begin(), nums.end(), 0);
		if (num_total != 0) res.emplace_back(to_string(num_total));
		return res;
	}

	void eval_s(const string& s, unordered_map<string, int>& m) {
		int coefficient = GetNumber(s);
		if (coefficient == 0) return;
		const int len = s.length();
		vector<string> s_v;
		string tmp = "";
		for (const auto& ch : s) {
			if (isalpha(ch)) {
				tmp += ch;
			}
			else if (!tmp.empty()) {
				s_v.emplace_back(tmp);
				tmp = "";
			}
		}
		if (!tmp.empty()) s_v.emplace_back(tmp);
		sort(s_v.begin(), s_v.end());
		string res = "";
		for (const auto& str : s_v) res += '*' + str;
		m[res] += coefficient;
	}

	bool IsNumber(const string& s) {
		for (const auto& ch : s) {
			if (isalpha(ch)) return false;
		}
		return true;
	}

	int GetNumber(const string& s) {
		deque<int> st;
		char op = '+';
		int sign = 1;
		const int len = s.length();
		for (int i = 0; i < len; ++i) {
			if (s[i] == '-') sign = -1;
			if (!isdigit(s[i])) continue;
			int num = 0;
			while (i < len && isdigit(s[i])) num = num * 10 + s[i++] - '0';
			num *= sign;
			sign = 1;
			switch (op) {
			case '+': st.emplace_back(num); break;
			case '-': st.emplace_back(-num); break;
			case '*': st.back() *= num; break;
			}
			if (i < len) op = s[i];
		}
		return accumulate(st.begin(), st.end(), 0);
	}

	static bool cmp(const string& s1, const string& s2) {
		string res1 = "", res2 = "";
		int degree1 = 1, degree2 = 1;
		for (const auto& ch : s1) {
			if (isalpha(ch) || ch == '*') res1 += ch;
			if (ch == '*') ++degree1;
		}
		for (const auto& ch : s2) {
			if (isalpha(ch) || ch == '*') res2 += ch;
			if (ch == '*') ++degree2;
		}
		return degree1 > degree2 || ((degree1 == degree2) && (res1 < res2));
	}
}; 

int main() {
	Solution s;
	auto s_v = s.basicCalculatorIV("(12 + 12) * 9 * ak", { }, {});
	for (const auto& s : s_v) cout << s << '\n';
	system("PAUSE");
}