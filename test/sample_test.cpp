// test/sample_test.cpp
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

#include <gtest/gtest.h>

#include <filesystem>

#include "befunge/befunge.hpp"

// TEST(SampleTest, 1) {
//   using namespace std;

//   stringstream ss;
//   istringstream is;
//   ostringstream os, ops;

//   ss << "v @_       v\n"
//      << ">0\"!dlroW\"v \n"
//      << "v  :#     < \n"
//      << ">\" ,olleH\" v\n"
//      << " ^       <  ";

//   BefungeInterpreter::Befunge b(ss, is, os);
//   b.run();
//   EXPECT_EQ("Hello, World!", os.str());
// }

TEST(SampleTest, 1) {
  using namespace std;

  ifstream ifp("../sample/01.befunge"), otp("../sample/01_o.txt");
  istringstream is;
  ostringstream os, ots;
  if (ifp && otp) {
    BefungeInterpreter::Befunge b(ifp, is, os);
    b.run();
    char tmp;
    while (otp.get(tmp)) ots << tmp;
    EXPECT_EQ(ots.str(), os.str());
  } else
    cerr << "Failed to open file." << endl;
  ifp.close();
  otp.close();
}

TEST(SampleTest, 2) {
  using namespace std;

  ifstream ifp("../sample/02.befunge"), otp("../sample/02_o.txt");
  istringstream is;
  ostringstream os, ots;
  if (ifp && otp) {
    BefungeInterpreter::Befunge b(ifp, is, os);
    b.run();
    char tmp;
    while (otp.get(tmp)) ots << tmp;
    EXPECT_EQ(ots.str(), os.str());
  } else
    cerr << "Failed to open file." << endl;
  ifp.close();
  otp.close();
}
