#include <Halide.h>

class GameOfLifeTask : public Halide::Generator<GameOfLifeTask> {
  public:
  Halide::GeneratorInput<Halide::Buffer<std::uint8_t>> input{"input", 2};
  Halide::GeneratorOutput<Halide::Buffer<std::uint8_t>> output{"output", 2};

  Halide::Var x{"x"}, y{"y"}, c{"c"};
  Halide::Func sum{"sum"};

  void generate() {
    auto input_bordered = Halide::BoundaryConditions::constant_exterior(input, 0);
    auto r = Halide::RDom(-1, 3, -1, 3);

    // sum(x, y) = Halide::sum(input_bordered(x + r.x, y + r.y));
    // output(x, y) = Halide::select(
    //   sum(x, y) == 3, static_cast<std::uint8_t>(1),
    //   sum(x, y) == 4, input_bordered(x, y),
    //   static_cast<std::uint8_t>(0)
    // );

    sum(x, y) = Halide::sum(Halide::select(
      r.x == 0 && r.y == 0, Halide::ConciseCasts::u8(0),
      input_bordered(x + r.x, y + r.y)
    ));

    output(x, y) = Halide::select(
      input(x, y) == 0 && sum(x, y) == 3, Halide::ConciseCasts::u8(1),
      input(x, y) == 1 && (sum(x, y) == 2 || sum(x, y) == 3), Halide::ConciseCasts::u8(1),
      Halide::ConciseCasts::u8(0)
    );
  }

  void schedule() {
    output.reorder(x, y);
    sum.reorder(x, y);
  }
};

HALIDE_REGISTER_GENERATOR(GameOfLifeTask, gameoflife_task);

