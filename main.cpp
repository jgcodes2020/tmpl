#include <tmpl/unroll.hpp>
#include <iostream>
#include <cstddef>

using std::cout;
using std::endl;

int main() {
  tmpl::unroll_loop<size_t, 20>([&](size_t idx) {
    cout << "Loop " << idx << std::endl;
  });
}