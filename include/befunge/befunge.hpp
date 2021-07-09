// include/befunge/befunge.hpp
//
// Copyright 2021 wsbky
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

namespace BefungeInterpreter {
struct out_of_range : public std::out_of_range {
  using std::out_of_range::out_of_range;
};
struct Befunge {
 private:
  using befungeNumber = int;
  struct BefungeStack {
    using stk_type = std::stack<befungeNumber>;
    using size_type = int;
    constexpr static size_type stack_lim = 30000;
    stk_type s;
    void push(befungeNumber a) {
      s.push(a);
      if (s.size() >= stack_lim) throw out_of_range("Stack overflow.");
    }
    befungeNumber top() { return (s.empty() ? 0 : s.top()); }
    void pop() {
      if (!s.empty()) s.pop();
    }
    void add() {
      befungeNumber y = top();
      pop();
      befungeNumber x = top();
      pop();
      s.push(x + y);
    }
    void sub() {
      befungeNumber y = top();
      pop();
      befungeNumber x = top();
      pop();
      s.push(x - y);
    }
    void mul() {
      befungeNumber y = top();
      pop();
      befungeNumber x = top();
      pop();
      s.push(x * y);
    }
    void div() {
      befungeNumber y = top();
      pop();
      befungeNumber x = top();
      pop();
      s.push(x / y);
    }
    void mod() {
      befungeNumber y = top();
      pop();
      befungeNumber x = top();
      pop();
      s.push(x % y);
    }
    void pos() {
      befungeNumber y = top();
      pop();
      befungeNumber x = top();
      pop();
      s.push((x > y) ? 1 : 0);
    }
    void not_() {
      befungeNumber tmp = (top() == 0) ? 1 : 0;
      pop();
      s.push(tmp);
    }
    void dup() {
      befungeNumber tmp = top();
      pop();
      s.push(tmp);
      s.push(tmp);
    }
    void rev() {
      befungeNumber y = s.top();
      pop();
      befungeNumber x = s.top();
      pop();
      s.push(y);
      s.push(x);
    }
    void dmp() { pop(); }
  };
  struct BefungeTable {
    using befungeNumber = int;
    using num_type = int;
    using size_type = int;
    using tab_type = std::vector<std::vector<befungeNumber>>;
    using pt_type = std::pair<int, int>;
    using tbsize_type = std::pair<int, int>;
    constexpr static size_type stack_lim = 30000;
    constexpr static size_type tblen_lim = 100;

    tab_type t;
    pt_type pt;
    tbsize_type sz;
    BefungeStack s;
    bool astack_mode;
    std::pair<int, int> direction;
    BefungeTable()
        : pt(std::make_pair(0, 0)),
          sz(std::make_pair(0, 0)),
          astack_mode(false),
          direction(std::make_pair(1, 0)) {}
    void set(std::istream &ss) {
      std::vector<std::string> lines;
      std::vector<int> line_sizes;
      std::string line;
      while (std::getline(ss, line)) lines.push_back(line);
      int tab_height = (int)lines.size();
      t.reserve(tab_height);
      line_sizes.reserve(tab_height);
      for (const auto &l : lines) line_sizes.push_back((int)l.size());
      int tab_width = *std::max_element(line_sizes.begin(), line_sizes.end());
      for (const auto &l : lines) {
        num_type i = 0;
        std::vector<befungeNumber> tmp(tab_width, -1);
        for (const auto &c : l) tmp[i++] = befungeNumber(c);
        t.push_back(tmp);
      }
      sz = std::make_pair(tab_width, tab_height);
      direction = std::make_pair(1, 0);
    }
    const auto &cur() { return t[pt.second][pt.first]; }
    const auto &get(int x, int y) { return t[y][x]; }
    void nxt() { nxt(1); }
    void nxt(num_type a) {
      pt.first += direction.first * a;
      pt.second += direction.second * a;
      if (pt.first >= sz.first)
        pt.first -= sz.first;
      else if (pt.first < 0)
        pt.first += sz.first;
      else if (pt.second >= sz.second)
        pt.second -= sz.second;
      else if (pt.second < 0)
        pt.second += sz.second;
    }
    void fnd() {
      befungeNumber y = s.top();
      s.pop();
      befungeNumber x = s.top();
      s.pop();
      s.push(get(x, y));
    }
    void rpl() {
      num_type y = s.top();
      s.pop();
      num_type x = s.top();
      s.pop();
      num_type v = s.top();
      s.pop();
      t[y][x] = v;
    }
    void dqt() { astack_mode = !astack_mode; }
    void stk() { s.push(cur()); }
    void set_direction(int dx, int dy) { direction = std::make_pair(dx, dy); }
  };
  BefungeTable t;
  std::istream &is;
  std::ostream &os;
  void get_dgt() {
    std::string s;
    is >> s;
    num_type n = stoi(s);
    t.s.push(n);
  }
  void get_chr() {
    num_type n = is.get();
    t.s.push(n);
  }
  void put_dgt() {
    os << t.s.top();
    t.s.pop();
  }
  void put_chr() {
    os.put((char)t.s.top());
    t.s.pop();
  }

 public:
  using cnt_type = int;
  using debug_type = int;
  using num_type = int;

  // control
  constexpr static char c_rt = '>', c_lf = '<', c_up = '^', c_dn = 'v',
                        c_pph = '_', c_ppv = '|', c_rnd = '?', c_nth = ' ',
                        c_ign = '#', c_stp = '@', c_dqt = '"';
  // input and output
  constexpr static char i_num = '&', i_chr = '~', o_num = '.', o_chr = ',';
  // operators
  constexpr static char op_add = '+', op_sub = '-', op_mul = '*', op_div = '/',
                        op_mod = '%', op_pos = '`', op_not = '!';
  // stack control
  constexpr static char s_dup = ':', s_rev = '\\', s_dmp = '$';
  // memory control
  constexpr static char m_fnd = 'g', m_rpl = 'p';
  Befunge(std::istream &ism = std::cin, std::istream &i = std::cin,
          std::ostream &o = std::cout)
      : is(i), os(o) {
    t.set(ism);
    std::srand((unsigned int)time(NULL));
  }
  bool step() {
    num_type a = 1;
    befungeNumber cur = t.cur();
    if (t.astack_mode && cur != -1 && cur != '"')
      t.s.push(cur);
    else
      switch (cur) {
        case c_rt: t.set_direction(1, 0); break;
        case c_lf: t.set_direction(-1, 0); break;
        case c_up: t.set_direction(0, -1); break;
        case c_dn: t.set_direction(0, 1); break;
        case c_pph:
          t.set_direction((t.s.top() == 0 ? 1 : -1), 0);
          t.s.pop();
          break;
        case c_ppv:
          t.set_direction(0, (t.s.top() == 0 ? 1 : -1));
          t.s.pop();
          break;
        case c_rnd:
          switch (rand() % 4) {
            case 0: t.set_direction(1, 0); break;
            case 1: t.set_direction(-1, 0); break;
            case 2: t.set_direction(0, -1); break;
            case 3: t.set_direction(0, 1); break;
          }
          break;
        case c_nth: break;
        case c_ign: a = 2; break;
        case c_stp: return false; break;
        case c_dqt: t.dqt(); break;
        case i_num: get_dgt(); break;
        case i_chr: get_chr(); break;
        case o_num: put_dgt(); break;
        case o_chr: put_chr(); break;
        case op_add: t.s.add(); break;
        case op_sub: t.s.sub(); break;
        case op_mul: t.s.mul(); break;
        case op_div: t.s.div(); break;
        case op_mod: t.s.mod(); break;
        case op_pos: t.s.pos(); break;
        case op_not: t.s.not_(); break;
        case s_dup: t.s.dup(); break;
        case s_rev: t.s.rev(); break;
        case s_dmp: t.s.dmp(); break;
        case m_fnd: t.fnd(); break;
        case m_rpl: t.rpl(); break;
        default:
          if (cur >= '0' && cur <= '9') t.s.push(cur - '0');
          break;
      }
    t.nxt(a);
    return true;
  }
  void run() {
    while (step())
      ;
  }
};

}  // namespace BefungeInterpreter
