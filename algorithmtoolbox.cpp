#include "algorithmtoolbox.h"

AlgorithmToolBox::AlgorithmToolBox(QWidget *parent)
    : QWidget{parent}
{
    Algorithm *algo_fft = new Algorithm();
    algo_fft->setText("FFT");
    algo_fft->setIcon(QIcon(":/res/icons/fft.ico"));
}

Algorithm::Algorithm(QPushButton *parent) : QPushButton{parent} {}
