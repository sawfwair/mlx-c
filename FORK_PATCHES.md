# MLX C fork patches

Upstream cutoff: `57731b00e8bed710e0750e7356da9f18f8eb3e20` (MLX v0.32.4 bindings).

The private I/O adapters construct error messages with `std::string` and
`std::to_string` instead of `std::format`. This preserves the exact diagnostics
while supporting the libstdc++ shipped with Ubuntu 22.04/Jammy. I/O callbacks,
exception types, public APIs, and tensor execution are unchanged.

`-DMLX_C_BUILD_TESTS=ON` enables `mlx_c_io_errors`. Run it with
`ctest --test-dir build --output-on-failure`. The test covers short sequential
reads, short positioned reads, short writes, and reader/writer seek failures.
