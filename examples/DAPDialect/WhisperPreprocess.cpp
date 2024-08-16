//===- WhisperPreprocessor.cpp --------------------------------------------===//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//
//
// This file implements the benchmark for Whisper Preprocessor function.
//
//===----------------------------------------------------------------------===//

#include <buddy/Core/Container.h>
#include <buddy/DAP/DAP.h>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>

using namespace dap;
using namespace std;

constexpr size_t AudioDataLength = 93680;

/// Print [Log] label in bold blue format.
void printLogLabel() { std::cout << "\033[34;1m[Log] \033[0m"; }

int main() {

  /// Print the title of this example.
  const std::string title = "Whisper Preprocess Powered by Buddy Compiler";
  std::cout << "\033[33;1m" << title << "\033[0m" << std::endl;

  /// Initialize data containers
  //  - Input audio container: store raw audio data
  //  - Output container: audio features with memref<1x80x3000xf32>
  MemRef<double, 1> rawAudioContainer({AudioDataLength});
  intptr_t dataShape[1] = {AudioDataLength};
  rawAudioContainer =  std::move(MemRef<double, 1>(rawSpeech, dataShape));
  float *outputAlign = new float[240000];
  intptr_t sizesOutput[3] = {1, 80, 3000};
  MemRef<float, 3> outputContainer(outputAlign, sizesOutput);

  /// Calculate audioInput from rawAudioData.
  printLogLabel();
  std::cout << "Preprocessing audio..." << std::endl;
  const auto loadStart = std::chrono::high_resolution_clock::now();
  dap::whisperPreprocess(&rawAudioContainer, &outputContainer);
  const auto loadEnd = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double, std::milli> loadTime =
      loadEnd - loadStart;
  printLogLabel();
  std::cout << "Audio preprocess time: " << (double)(loadTime.count()) / 1000
            << "s\n"
            << std::endl;

  // /// Print output to txt.
  // ofstream fout("output.txt");
  // // Generate whisper preprocessor result.
  // fout << "-----------------------------------------" << std::endl;
  // fout << "[ whisper preprocessor result ]" << std::endl;
  // fout << "-----------------------------------------" << std::endl;
  // // Print reuslt data.
  // for (int i = 0; i < 240000; ++i) {
  //     fout << outputContainer[i] << std::endl;
  // }
  // fout.close();

  return 0;
}