#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "../src/nnet.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/shape.hpp>

using std::vector;
using std::cout;
using std::endl;
using nn::Layer;
using nn::InputLayer;
using nn::OutputLayer;
using nn::NeuralNet;
using nn::mat;
//using nn::ConvLayer;

const int datasize = 42000;
const int n = 784;

void load(mat &x, mat &y) {
  std::ifstream input("/home/joseph/Python/competition/digits/train.raw",
          std::ios::in);
  x = mat(datasize, n);
  y = mat(datasize, 10);
  y.zeros();
  if (input.is_open()) {
    cout << "reading data..." << endl;
    for (uint32_t i = 0 ; i < datasize ; ++i) {
      int yi = 0;
      input >> yi;
      y(i, yi) = 1;
      for (int j = 0 ; j < n ; ++j) {
        int xi = 0;
        input >> xi;
        x(i, j) = xi;
      }
    }
    cout << "done loading training data." << endl;
  }
}
void loadsample(mat &sample) {
  const int samplesize = 28000;
  std::ifstream input("/home/joseph/Python/competition/digits/test.raw",
          std::ios::in);
  if (input.is_open()) {
    cout << "reading testing data..." << endl;
    sample = mat(samplesize, n);
    for (int i = 0 ; i < samplesize ; ++i) {
      int xi = 0;
      for (int j = 0 ; j < n ; ++j) {
        input >> xi;
        sample(i, j) = xi;
      }
    }
    cout << "done loading testing samples." << endl;
  }
}

int main() {
  //const double lrate = 1e-2;
  //const double lambda = 1e1;
  //const int batchsize = 0;
  const int imagesize = 28;

  cv::Mat image = cv::imread("./data/bear.png", CV_LOAD_IMAGE_COLOR);
  cv::Mat resized = image(cv::Rect(100, 100, imagesize, imagesize));
  mat img(1, imagesize*imagesize*3);
  cout << "channels: " << resized.channels() << endl;
  cout << "rows: " << resized.rows << endl;
  cout << "cols: " << resized.cols << endl;
  for (int c = 0 ; c < resized.channels() ; ++c) {
    for (int i = 0 ; i < resized.rows ; ++i) {
      for (int j = 0 ; j < resized.cols ; ++j) {
        img(0, i * resized.cols + j + c * imagesize * imagesize) =
            resized.at<cv::Vec3b>(i, j).val[c];
      }
    }
  }

  srand(time(NULL));

  //InputLayer input(n);
  //vector<Layer> hidden = {
    //Layer(n, 100, lrate, lambda, sigmoid, sigmoidgrad),
  //};
  //OutputLayer output(100, 10, lrate, lambda, sigmoid, sigmoidgrad, cost, costd);
  //NeuralNet nnet(input, output, hidden);

  //mat x, y, sample;
  //load(x, y);
  //loadsample(sample);
  //nnet.feeddata(x.row(0), y.row(0), true);

  //ConvLayer layer(imagesize, imagesize, 3, 7, 1, 1, lrate, nn::relu);
  //mat result = layer.forwardprop(img);
  //cout << result.n_rows << endl;
  //cout << result.n_cols << endl;
  //cout << layer.forwardprop(x.row(0)) << endl;

  //cout << "training start..." << endl;
  //for (int i = 0 ; i < datasize * 26; ++i) {
    //const int start = i % (datasize-batchsize);
    //const int end = start + batchsize;
    //nnet.feeddata(x.rows(start, end), y.rows(start, end), false);

    //if (i % datasize == 0) {
      //const double cost = nnet.computecost();
      //if (cost < 0.01) break;
      //cout << "iteration: " << i << " cost: " << cost << endl;
      //cout << nnet.getresult() << endl;
      //cout << y.rows(start, end) << endl;
    //}
  //}
  //cout << endl << "training completed" << endl;

  //for (uint32_t i = 0 ; i < x.n_rows ; ++i) {
    //const mat xi = x.row(i);
    //const mat result = nnet.predict(xi);
    //cout << nnet.getresult() << endl;

    //cv::Mat image = cv::Mat::zeros(imagesize, imagesize, CV_8UC1);
    //for (uint32_t i = 0 ; i < xi.n_cols ; ++i) {
      //image.at<uchar>(i/imagesize, i%imagesize) = xi(i);
    //}

    //int answer = 0;
    //for (uint32_t j = 0 ; j < 10 ; j ++) {
      //if (y(i, j) == 1) {
        //answer = j;
        //break;
      //}
    //}
    //cv::Mat large;
    //cv::resize(image, large, cv::Size(256, 256));
    //cv::imshow("Hand Written digits", large);
    //cout << "prediction: " << result(0, 0) << " answer: " << answer << endl;
    //int c = cv::waitKey(0);
    //if (c == 'q' || c == 'Q') break;
  //}
  return 0;
}
