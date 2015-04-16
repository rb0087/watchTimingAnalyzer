#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
  //  plotSineWave(100.0);
   // plotSineWave(50.0);
    //plotSineWave(10.0);
    //plotSineWave(30.0);
   plotSineWave(24.0);
    plotSineWave(25.0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plotSineWave(double omega)
{
    QVector<double> t(44100);
    QVector<double> y(44100);
    double dt = 1/44100.0;
    for(int i=0;i<t.lastIndexOf(0);i++)
    {
        t[i]=dt*i;
        y[i]=sin(t[i]*omega)/(omega*t[i]);
    }
    QCPGraph *thisGraph;
    thisGraph = ui->plotWidget->addGraph();
    thisGraph->setData(t,y);
    ui->plotWidget->xAxis->setLabel("time, seconds");
    ui->plotWidget->yAxis->setLabel("Magnitude");
    ui->plotWidget->xAxis->setRange(0,1);
    ui->plotWidget->yAxis->setRange(-1,1);
    ui->plotWidget->replot();
}

void MainWindow::on_pushButton_clicked()
{
    double frequency = ui->frequencyInput->value();
    this->plotSineWave(frequency);
    //ui->label_2->setText("Temp");
}
