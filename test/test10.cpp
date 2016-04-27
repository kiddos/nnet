#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/shape.hpp>

#include "../src/nnet.h"

using std::vector;
using std::cout;
using std::endl;
using nn::Layer;
using nn::InputLayer;
using nn::OutputLayer;
using nn::NeuralNet;
using nn::mat;

const int datasize = 4177;
const int n = 8;

double rectifier(double z) {
  return z >= 0 ? z : 0;
}

double rectifiergrad(double z) {
  return z >= 0 ? 1 : 0;
}

double sigmoid(double z) {
  return 1.0 / (1.0 + exp(-z));
}

double sigmoidgrad(double z) {
  const double e = exp(-z);
  const double b = 1 + e;
  return e / (b * b);
}

double identity(double z) {
  return z;
}

double identitygrad(double) {
  return 1;
}


mat colsum(const mat m) {
  mat result(m.n_rows, m.n_cols);
  result.zeros();

  for (uint32_t i = 0 ; i < m.n_rows ; ++i) {
    double sum = 0;
    for (uint32_t j = 0 ; j < m.n_cols ; ++j) {
      sum += m(i, j);
    }
    for (uint32_t j = 0 ; j < m.n_cols ; ++j) {
      result(i, j) = sum;
    }
  }
  return result;
}

mat cost(mat y, mat h) {
  //const mat exponential = nn::funcop(h, exp);
  //const mat sum = colsum(exponential);
  //const mat p = exponential % (1 / sum);
  //const mat J = - (y % nn::funcop(p, log));
  mat J = -(y % nn::funcop(h, log) + (1-y) % nn::funcop(1-h, log));
  //mat J = -(y % h.transform(log) + (1-y) % nn::funcop(1-h, log)) / y.n_rows;
  //mat J = -(y % h);
  return J;
}

mat costd(mat y, mat a, mat,mat) {
  //const mat exponential = nn::funcop(a, exp);
  //const mat sum = colsum(exponential);
  //const mat p = exponential % (1.0 / sum);
  //const mat delta = p - y;
  mat delta = (a - y);
  //mat delta = (a - y) / y.n_rows;
  //mat grad = -(y % nn::funcop(z, sigmoidgrad));
  return delta;
}

void load(mat &x, mat &y) {
  std::ifstream input("/home/joseph/C/data/abalone.dat", std::ios::in);
  std::string line;

  if (input.is_open()) {
    x = mat(datasize, n);
    y = mat(datasize, 29);
    y.zeros();

    for (; std::getline(input, line) ;) {
      if (strcmp(line.substr(0, 5).c_str(), "@data") == 0) {
        cout << "reading data..." << endl;
        break;
      }
    }

    for (uint32_t i = 0 ; i < datasize ; ++i) {
      for (int j = 0 ; j < n ; ++j) {
        double xi = 0;
        char c;
        input >> xi;
        x(i, j) = xi;

        // read ,
        input >> c;
      }

      double val = 0;
      input >> val;
      y(i, val-1) = 1;
    }
  }
}

double accuracy(mat answer, mat prediction) {
  double correct = 0;
  for (uint32_t i = 0 ; i < answer.n_rows ; ++i) {
    const int index = static_cast<int>(prediction(i, 0));
    if (answer(i, index) == 1) {
      correct ++;
    }
  }
  return correct / static_cast<double>(answer.n_rows);
}

int main() {
  double lrate = 1e-1;
  //const double lratedecay = 0.66;
  const double lambda = 1e-1;

  srand(time(NULL));

  InputLayer input(n);
  vector<Layer> hidden = {
    Layer(n, 1024, lrate, lambda, atan, [] (double x) {return 1.0/(1.0+x*x);}),
    Layer(1024, 64, lrate, lambda, sigmoid, sigmoidgrad),
    Layer(64, 16, lrate, lambda, sigmoid, sigmoidgrad),
  };
  OutputLayer output(16, 29, lrate, 0, sigmoid, sigmoid, cost, costd);
  NeuralNet nnet(input, output, hidden);

  mat x, y;
  load(x, y);
  //cout << x << endl;
  //cout << y << endl;

  //nnet.feeddata(x.row(3), y.row(3), true);
  for (int i = 0 ; i < 320000 ; ++i) {
    nnet.feeddata(x.row(i%datasize), y.row(i%datasize), false);
    //nnet.feeddata(x, y, false);
    cout << "\riteration: " << i+1 << " cost: " << nnet.computecost();
    if (i % datasize == 0) {
      cout << endl << "iteration: " << i+1 << " cost: " << nnet.computecost();
      cout << endl << nnet.getresult() << endl;
      cout << y.row(i% datasize) << endl;
    }
    //if (i % 5000 == 0) {
      //lrate *= lratedecay;
      //nnet.setlrate(lrate);
    //}
  }
  cout << endl;
  mat result = nnet.predict(x);
  cout << "accuracy: " << accuracy(y, result) * 100 << endl;

  return 0;
}
