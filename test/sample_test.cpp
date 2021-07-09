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
#include <regex>

#include "befunge/befunge.hpp"

using std::filesystem::path;

std::vector<std::pair<std::string, std::string>> getFileList() {
  // std::vector<std::pair<path&, path&>> ret;
  std::vector<std::pair<std::string, std::string>> ret;
  std::vector<std::string> fa;
  std::vector<std::string> fb;
  ret.reserve(100);
  fa.reserve(100);
  fb.reserve(100);
  std::regex re_tx(".*_o\\.txt");

  for (const auto& file : std::filesystem::directory_iterator("../sample")) {
    path fp = file.path();
    if (fp.extension() == ".befunge")
      fa.push_back(fp.filename().string());
    else if (std::regex_match(fp.filename().string(), re_tx))
      fb.push_back(fp.filename().string());
  }
  std::sort(fa.begin(), fa.end());
  std::sort(fb.begin(), fb.end());
  for (int i = 0; i < (int)std::min(fa.size(), fb.size()); ++i) {
    std::string sa = fa[i];
    std::string sb = fb[i];
    if (sa.substr(0, sa.length() - 8) == sb.substr(0, sb.length() - 6))
      ret.push_back(std::make_pair(fa[i], fb[i]));
  }
  if (ret.size() == 0)
    std::cerr << "Couldn't generate list. Make sure you're following the rule "
                 "in directory ../sample/ ."
              << std::endl;
  return ret;
}

class SampleTest
    : public ::testing::TestWithParam<std::pair<std::string, std::string>> {};

TEST_P(SampleTest, TestA) {
  std::string pa = "../sample/" + GetParam().first;
  std::string pb = "../sample/" + GetParam().second;

  std::ifstream ifp(pa), otp(pb);
  std::istringstream is;
  std::ostringstream os, ots;
  if (ifp && otp) {
    BefungeInterpreter::Befunge b(ifp, is, os);
    b.run();
    char tmp;
    while (otp.get(tmp)) ots << tmp;
    EXPECT_EQ(ots.str(), os.str());
  } else
    std::cerr << "Failed to open file." << std::endl;
  ifp.close();
  otp.close();
}

INSTANTIATE_TEST_SUITE_P(MyTests, SampleTest,
                         ::testing::ValuesIn(getFileList()));
