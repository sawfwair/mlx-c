/* Copyright © 2026 MLX C contributors. */

#include <iostream>
#include <stdexcept>
#include <string>

#include "mlx/c/mlx.h"
#include "mlx/c/private/io.h"

namespace {

template <typename F>
void expect_error(F&& operation, const std::string& expected) {
  try {
    operation();
  } catch (const std::runtime_error& error) {
    if (error.what() == expected) {
      return;
    }
    throw std::runtime_error(
        "unexpected diagnostic: " + std::string(error.what()));
  }
  throw std::runtime_error("expected diagnostic: " + expected);
}

} // namespace

int main() {
  mlx_io_vtable vtable{};
  vtable.read = [](void*, char*, size_t) -> size_t { return 2; };
  vtable.read_at_offset = [](void*, char*, size_t, size_t) -> size_t {
    return 3;
  };
  vtable.write = [](void*, const char*, size_t) -> size_t { return 4; };
  vtable.seek = [](void*, int64_t, int) { return -1; };
  vtable.label = [](void*) -> const char* { return "fixture"; };
  vtable.free = [](void*) {};
  CReader reader(nullptr, vtable);
  CWriter writer(nullptr, vtable);
  char data[12]{};

  expect_error(
      [&] { reader.read(data, 12); },
      "[mlx_io_reader] unable to read 12 bytes (read 2 instead) in fixture");
  expect_error(
      [&] { reader.read(data, 12, 20); },
      "[mlx_io_reader] unable to read 12 bytes (read 3 instead) in fixture");
  expect_error(
      [&] { writer.write(data, 12); },
      "[mlx_io_writer] unable to write 12 bytes (wrote 4 instead) in fixture");
  expect_error(
      [&] { reader.seek(0); }, "[mlx_io_reader] unable to seek in fixture");
  expect_error(
      [&] { writer.seek(0); }, "[mlx_io_writer] unable to seek in fixture");
  std::cout << "I/O error diagnostics pass\n";
}
