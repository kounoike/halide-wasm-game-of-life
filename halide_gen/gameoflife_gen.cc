#include <Halide.h>

class GameOfLifeTask : public Halide::Generator<GameOfLifeTask>
{
public:
  Halide::GeneratorInput<Halide::Buffer<std::uint8_t>> input{"input", 2};
  Halide::GeneratorOutput<Halide::Buffer<std::uint8_t>> output{"output", 2};

  Halide::Var x{"x"}, y{"y"}, c{"c"}, xi{"xi"}, yi{"yi"}, yo{"yo"};
  Halide::Func sum{"sum"}, sumx{"sumx"}, sumy{"sumy"};

  void generate()
  {
    auto input_bordered = Halide::BoundaryConditions::repeat_image(input);
    auto rd = Halide::RDom(-1, 3);

    // sum(x, y) = Halide::sum(Halide::select(
    //   r.x == 0 && r.y == 0, Halide::ConciseCasts::u8(0),
    //   input_bordered(x + r.x, y + r.y)
    // ));
    // output(x, y) = Halide::select(
    //     input(x, y) == 0 && sum(x, y) == 3, Halide::ConciseCasts::u8(1),
    //     input(x, y) == 1 && (sum(x, y) == 2 || sum(x, y) == 3), Halide::ConciseCasts::u8(1),
    //     Halide::ConciseCasts::u8(0));

    // sum(x, y) = 2 * Halide::sum(input_bordered(x + r.x, y + r.y)) - input_bordered(x, y);
    // sum(x, y) += 2 * input_bordered(x + r.x, y + r.y);
    // sum(x, y) -= input_bordered(x, y);
    // output(x, y) = Halide::ConciseCasts::u8((sum(x, y) > 4) && (sum(x, y) < 8));
    sumx(x, y) = Halide::sum(input_bordered(x + rd, y));
    sumy(x, y) = Halide::sum(sumx(x, y + rd));
    sum(x, y) = 2 * sumy(x, y) - input_bordered(x, y);
    output(x, y) = Halide::ConciseCasts::u8(sum(x, y) > 4 && sum(x, y) < 8);
  }

  void schedule()
  {
    output.set_estimates({{0, 16 * 1024}, {0, 16 * 1024}});
    input.set_estimates({{0, 16 * 1024}, {0, 16 * 1024}});
    // output.split(y, yo, yi, 4);
    // output.reorder(yi, x, yo);
    // output.unroll(yi);
    // sum.compute_at(output, y);
    // sumy.compute_at(sum, y);
    // sumx.compute_at(sumy, x).store_at(sumy, x);

    // sum.reorder(x, y);
    // output.reorder(x, y);
  }
};

HALIDE_REGISTER_GENERATOR(GameOfLifeTask, gameoflife_task);
