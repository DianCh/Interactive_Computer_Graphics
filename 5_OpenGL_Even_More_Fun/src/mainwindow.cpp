#include "mainwindow.h"
#include <ui_mainwindow.h>
#include <QDesktopWidget>
#include <QResizeEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    shaderControls(new ShaderControls())
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(shaderControls, SIGNAL(sig_setCurrentModel(int)), ui->mygl, SLOT(slot_setCurrentModel(int)));
    connect(shaderControls, SIGNAL(sig_setCurrentSurfaceShaderProgram(int)), ui->mygl, SLOT(slot_setCurrentSurfaceShaderProgram(int)));
    connect(shaderControls, SIGNAL(sig_setCurrentPostprocessShaderProgram(int)), ui->mygl, SLOT(slot_setCurrentPostprocessShaderProgram(int)));
    connect(shaderControls, SIGNAL(sig_setCurrentMatcapTexture(int)), ui->mygl, SLOT(slot_setCurrentMatcapTexture(int)));

    shaderControls->show();
    shaderControls->move(QApplication::desktop()->screen()->rect().center() - this->rect().center() + QPoint(this->devicePixelRatio() * this->width(), 0));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete shaderControls;
}

void MainWindow::on_actionQuit_triggered()
{
    shaderControls->close();
    QApplication::exit();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    shaderControls->close();
    QMainWindow::closeEvent(e);
}
