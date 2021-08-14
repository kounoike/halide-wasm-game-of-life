#include <Halide.h>

class VisualizeTask : public Halide::Generator<VisualizeTask> {
  public:
  Halide::GeneratorInput<Halide::Buffer<std::uint8_t>> input{"input", 2};
  Halide::GeneratorOutput<Halide::Buffer<std::uint8_t>> output{"output", 3};
  Halide::GeneratorInput<int> size{"size"};

  Halide::Var x{"x"}, y{"y"}, c{"c"};

  void generate() {
    output(x, y, c) = Halide::select(
      c == 3, (uint8_t)255,
      // (x % size == 0) || (x % size == size - 1) || (y % size == 0) || (y % size == size - 1), (uint8_t) 128,
      (size >= 3) && ((x % size == 0) || (y % size == 0)), (uint8_t) 128,
      (1 - input(x / size, y / size)) * (uint8_t) 245 + (uint8_t) 5
    );
  }

  void schedule() {
    output.dim(0).set_stride(4);
    output.dim(2).set_stride(1);
    output.dim(2).set_bounds(0, 4);

    output.reorder(c, x, y).unroll(c);
  }
};

HALIDE_REGISTER_GENERATOR(VisualizeTask, visualize_task);

