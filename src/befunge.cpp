// src/befunge.cpp
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

#include "befunge/befunge.hpp"

#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
  using namespace std;
  if (argc > 1)
    for (int i = 1; i < argc; ++i) {
      ifstream ifs(argv[i]);
      if (ifs) {
        BefungeInterpreter::Befunge b(ifs);
        b.run();
      } else
        std::cerr << "An error occurred whlie reading file " << argv[i]
                  << "; the file possibly does not exist.\n";
      ifs.close();
    }
  else
    BefungeInterpreter::Befunge().run();
  return 0;
}
