#include <gtest/gtest.h>

#include "visualize.h"
#include "gameoflife.h"
#include <HalideBuffer.h>

// Demonstrate some basic assertions.
TEST(VisualizeTest, BasicAssertions)
{
  const int size = 8;
  const int width = 4;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  auto output = Halide::Runtime::Buffer<uint8_t>::make_interleaved(width * size, height * size, 4);

  input(0, 0) = 0;
  input(1, 0) = 0;
  input(2, 0) = 0;
  input(3, 0) = 0;

  input(0, 1) = 0;
  input(1, 1) = 0;
  input(2, 1) = 0;
  input(3, 1) = 0;

  input(0, 2) = 0;
  input(1, 2) = 0;
  input(2, 2) = 0;
  input(3, 2) = 0;

  visualize(input, size, output);
  for (int c = 0; c < 3; ++c)
  {
    EXPECT_EQ(output(0, 0, c), 128);
    EXPECT_EQ(output(0, 1, c), 128);
    EXPECT_EQ(output(1, 0, c), 128);

    // EXPECT_EQ(output(size - 1, 0, c), 250);
    // EXPECT_EQ(output(size - 1, 1, c), 128);
    // EXPECT_EQ(output(size - 2, 0, c), 128);

    // EXPECT_EQ(output(0, size - 1, c), 128);
    // EXPECT_EQ(output(1, size - 1, c), 128);
    // EXPECT_EQ(output(0, size - 2, c), 128);

    // EXPECT_EQ(output(size - 1, size - 1, c), 128);
    // EXPECT_EQ(output(size - 2, size - 1, c), 128);
    // EXPECT_EQ(output(size - 1, size - 2, c), 128);

    // EXPECT_EQ(output(1, 1, c), 250);
    // EXPECT_EQ(output(1, 2, c), 250);
    // EXPECT_EQ(output(2, 1, c), 250);
    // EXPECT_EQ(output(2, 2, c), 250);
  }
}

// 合計0
TEST(GameOfLifeTest, Rule0)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 0;
  input(0, 1) = 0;
  input(0, 2) = 0;

  input(1, 0) = 0;
  input(1, 1) = 0;
  input(1, 2) = 0;

  input(2, 0) = 0;
  input(2, 1) = 0;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計1、自分（中心）が0
TEST(GameOfLifeTest, Rule1_0)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 0;
  input(0, 1) = 1;
  input(0, 2) = 0;

  input(1, 0) = 0;
  input(1, 1) = 0;
  input(1, 2) = 0;

  input(2, 0) = 0;
  input(2, 1) = 0;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計1、自分（中心）が1
TEST(GameOfLifeTest, Rule1_1)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 0;
  input(0, 1) = 0;
  input(0, 2) = 0;

  input(1, 0) = 0;
  input(1, 1) = 1;
  input(1, 2) = 0;

  input(2, 0) = 0;
  input(2, 1) = 0;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計2、自分（中心）が0
TEST(GameOfLifeTest, Rule2_0)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 0;
  input(0, 1) = 1;
  input(0, 2) = 0;

  input(1, 0) = 0;
  input(1, 1) = 0;
  input(1, 2) = 0;

  input(2, 0) = 1;
  input(2, 1) = 0;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計2、自分（中心）が1
TEST(GameOfLifeTest, Rule2_1)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 0;
  input(0, 1) = 0;
  input(0, 2) = 0;

  input(1, 0) = 0;
  input(1, 1) = 1;
  input(1, 2) = 1;

  input(2, 0) = 0;
  input(2, 1) = 0;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計3、自分（中心）が0
TEST(GameOfLifeTest, Rule3_0)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 0;
  input(0, 1) = 1;
  input(0, 2) = 0;

  input(1, 0) = 1;
  input(1, 1) = 0;
  input(1, 2) = 0;

  input(2, 0) = 1;
  input(2, 1) = 0;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 1);
}

// 合計3、自分（中心）が1
TEST(GameOfLifeTest, Rule3_1)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 0;
  input(0, 1) = 1;
  input(0, 2) = 0;

  input(1, 0) = 0;
  input(1, 1) = 1;
  input(1, 2) = 1;

  input(2, 0) = 0;
  input(2, 1) = 0;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 1);
}

// 合計4、自分（中心）が0
TEST(GameOfLifeTest, Rule4_0)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 1;
  input(0, 2) = 0;

  input(1, 0) = 0;
  input(1, 1) = 0;
  input(1, 2) = 0;

  input(2, 0) = 1;
  input(2, 1) = 0;
  input(2, 2) = 1;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計4、自分（中心）が1
TEST(GameOfLifeTest, Rule4_1)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 0;
  input(0, 1) = 1;
  input(0, 2) = 1;

  input(1, 0) = 0;
  input(1, 1) = 1;
  input(1, 2) = 1;

  input(2, 0) = 0;
  input(2, 1) = 0;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 1);
}

// 合計5、自分（中心）が0
TEST(GameOfLifeTest, Rule5_0)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 1;
  input(0, 2) = 0;

  input(1, 0) = 1;
  input(1, 1) = 0;
  input(1, 2) = 1;

  input(2, 0) = 1;
  input(2, 1) = 0;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計5、自分（中心）が1
TEST(GameOfLifeTest, Rule5_1)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 0;
  input(0, 2) = 1;

  input(1, 0) = 0;
  input(1, 1) = 1;
  input(1, 2) = 1;

  input(2, 0) = 0;
  input(2, 1) = 1;
  input(2, 2) = 0;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計6、自分（中心）が0
TEST(GameOfLifeTest, Rule6_0)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 1;
  input(0, 2) = 1;

  input(1, 0) = 0;
  input(1, 1) = 0;
  input(1, 2) = 0;

  input(2, 0) = 1;
  input(2, 1) = 1;
  input(2, 2) = 1;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計6、自分（中心）が1
TEST(GameOfLifeTest, Rule6_1)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 0;
  input(0, 2) = 1;

  input(1, 0) = 0;
  input(1, 1) = 1;
  input(1, 2) = 1;

  input(2, 0) = 1;
  input(2, 1) = 0;
  input(2, 2) = 1;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計7、自分（中心）が0
TEST(GameOfLifeTest, Rule7_0)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 1;
  input(0, 2) = 0;

  input(1, 0) = 1;
  input(1, 1) = 0;
  input(1, 2) = 1;

  input(2, 0) = 1;
  input(2, 1) = 1;
  input(2, 2) = 1;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計7、自分（中心）が1
TEST(GameOfLifeTest, Rule7_1)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 1;
  input(0, 2) = 1;

  input(1, 0) = 0;
  input(1, 1) = 1;
  input(1, 2) = 1;

  input(2, 0) = 0;
  input(2, 1) = 1;
  input(2, 2) = 1;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計8、自分（中心）が0
TEST(GameOfLifeTest, Rule8_0)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 1;
  input(0, 2) = 1;

  input(1, 0) = 1;
  input(1, 1) = 0;
  input(1, 2) = 1;

  input(2, 0) = 1;
  input(2, 1) = 1;
  input(2, 2) = 1;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計8、自分（中心）が1
TEST(GameOfLifeTest, Rule8_1)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 1;
  input(0, 2) = 1;

  input(1, 0) = 0;
  input(1, 1) = 1;
  input(1, 2) = 1;

  input(2, 0) = 1;
  input(2, 1) = 1;
  input(2, 2) = 1;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}

// 合計9
TEST(GameOfLifeTest, Rule9)
{
  const int width = 3;
  const int height = 3;
  Halide::Runtime::Buffer<uint8_t> input(width, height);
  Halide::Runtime::Buffer<uint8_t> output(width, height);

  input(0, 0) = 1;
  input(0, 1) = 1;
  input(0, 2) = 1;

  input(1, 0) = 1;
  input(1, 1) = 1;
  input(1, 2) = 1;

  input(2, 0) = 1;
  input(2, 1) = 1;
  input(2, 2) = 1;

  gameoflife(input, output);

  EXPECT_EQ(output(1, 1), 0);
}
