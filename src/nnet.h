#ifndef NNET_H
#define NNET_H

#include <float.h>
#include <vector>
#include "type.h"
#include "layer.h"

namespace nn {

class NeuralNet {
 public:
  NeuralNet(const InputLayer input, const OutputLayer output,
            std::vector<Layer> layers);
  void feeddata(const mat x, const mat y, const bool check);
  mat predict(const mat sample);
  void gradcheck();
  double computecost();

 private:
  double computecost(const mat perturb, const uint32_t idx);
  mat computengrad(const int nrows, const int ncols, const int idx);

  const double eps;
  mat x, y;
  mat result;
  mat (*cost)(mat,mat);
  mat (*costd)(mat,mat,mat);
  InputLayer input;
  std::vector<Layer> hidden;
  OutputLayer output;
};

}

#endif /* end of include guard: NNET_H */

