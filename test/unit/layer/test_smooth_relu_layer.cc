#include <gtest/gtest.h>

#include "NerdNet/convert.h"
#include "NerdNet/except/nullptr_exception.h"
#include "NerdNet/layers.h"

using nerd::nn::VariableShape;
using nerd::nn::InputLayer;
using nerd::nn::SmoothReluLayer;
using nerd::nn::MeanSquareError;
using nerd::nn::Tensor;
using arma::Mat;

class SmoothReluLayerTest : public ::testing::Test {
 public:
  enum { INPUT_SIZE = 4, OUTPUT_SIZE = 8, DATA_SIZE = 4 };
  SmoothReluLayerTest()
      : smooth_relu_layer_(nullptr),
        error_layer_(nullptr, {INPUT_SIZE, OUTPUT_SIZE}) {}

 protected:
  void SetUp() override {
    // prepare test data
    test_data_ = Mat<float>(DATA_SIZE, INPUT_SIZE);
    test_data_.randn();

    test_label_ = Mat<float>(DATA_SIZE, OUTPUT_SIZE);
    test_label_.randn();
  }

  InputLayer input_layer_;
  SmoothReluLayer smooth_relu_layer_;
  MeanSquareError error_layer_;

  arma::Mat<float> test_data_;
  arma::Mat<float> test_label_;
};

TEST_F(SmoothReluLayerTest, Initialization) {
  bool exception_occur = false;
  try {
    error_layer_.Init();
  } catch (nerd::nn::except::NullPtrException& e) {
    exception_occur = true;
  }
  EXPECT_TRUE(exception_occur);

  smooth_relu_layer_ = SmoothReluLayer(&input_layer_);
  error_layer_ =
      MeanSquareError(&smooth_relu_layer_, {INPUT_SIZE, OUTPUT_SIZE});
  EXPECT_TRUE(error_layer_.Init());
}

TEST_F(SmoothReluLayerTest, DerivativeTest) {
  smooth_relu_layer_ = SmoothReluLayer(&input_layer_);
  error_layer_ =
      MeanSquareError(&smooth_relu_layer_, {INPUT_SIZE, OUTPUT_SIZE});
  error_layer_.Init();

  input_layer_.SetInput(test_data_);
  error_layer_.SetLabel(test_label_);

  error_layer_.ComputeCost();
  Tensor<float> deriv_tensor = error_layer_.ComputeDerivative();
  Mat<float> deriv;
  Tensor2Matrix(deriv_tensor, deriv);

  Mat<float> test_input = test_data_;
  Mat<float> test_deriv(test_input.n_rows, test_input.n_cols);
  constexpr float delta = 1e-4;
  constexpr float tol = 1e-1;
  for (int i = 0; i < DATA_SIZE; ++i) {
    for (int j = 0; j < INPUT_SIZE; ++j) {
      test_input(i, j) += delta;
      input_layer_.SetInput(test_input);
      float e1 = error_layer_.ComputeCost();

      test_input(i, j) -= 2 * delta;
      input_layer_.SetInput(test_input);
      float e2 = error_layer_.ComputeCost();

      test_input(i, j) += delta;

      test_deriv(i, j) = (e1 - e2) / (2 * delta);
      EXPECT_NEAR(test_deriv(i, j), deriv(i, j), tol);
    }
  }
}
