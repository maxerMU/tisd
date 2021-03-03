#include <QString>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QMessageBox>
#include <stdio.h>
#include "figure.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dx_input->setPlaceholderText(QString("dx"));
    ui->dy_input->setPlaceholderText(QString("dy"));
    ui->dz_input->setPlaceholderText(QString("dz"));
    ui->sxc_input->setPlaceholderText(QString("xc"));
    ui->syc_input->setPlaceholderText(QString("yc"));
    ui->szc_input->setPlaceholderText(QString("zc"));
    ui->kx_input->setPlaceholderText(QString("kx"));
    ui->ky_input->setPlaceholderText(QString("ky"));
    ui->kz_input->setPlaceholderText(QString("kz"));
    ui->rxc_input->setPlaceholderText(QString("xc"));
    ui->ryc_input->setPlaceholderText(QString("yc"));
    ui->rzc_input->setPlaceholderText(QString("zc"));
    ui->xy_ang_input->setPlaceholderText(QString("xy angle"));
    ui->xz_ang_input->setPlaceholderText(QString("xz angle"));
    ui->yz_ang_input->setPlaceholderText(QString("yz angle"));
    ui->infile_input->setPlaceholderText(QString("input file name"));
    ui->outfile_input->setPlaceholderText(QString("out file name"));

    scene = new QGraphicsScene();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete scene;
}


void MainWindow::on_read_btn_clicked()
{
    FILE *f = fopen(ui->infile_input->text().toStdString().c_str(), "r");
    if (!f)
        no_file_msg();
    else
    {
        int rc = figure_manager(READ, f);
        if (rc == EMPTY_FIGURE)
            wrong_format_msg();
        else
            draw();

        fclose(f);
    }
}

void MainWindow::no_file_msg()
{
    QMessageBox msg;
    msg.setText(QString("no such file"));
    msg.exec();
}

void MainWindow::wrong_format_msg()
{
    QMessageBox msg;
    msg.setText(QString("wrong format"));
    msg.exec();
}

void MainWindow::on_write_btn_clicked()
{
    FILE *f = fopen(ui->outfile_input->text().toStdString().c_str(), "w");
    figure_manager(WRITE, f);
    fclose(f);
}

void MainWindow::draw()
{
    QImage image(761, 751, QImage::Format_RGB32);
    QPainter painter(&image);
    painter.setPen(QColor(255, 255, 255));

    display_edge_t edge;
    int rc = figure_manager(GET_DISP_EDGE, 0, &edge);
    for (size_t i = 0; rc != WRONG_EDGE_INDEX; i++)
    {
        painter.drawLine(edge.x1, edge.y1, edge.x2, edge.y2);
        rc = figure_manager(GET_DISP_EDGE, i + 1, &edge);
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
    ui->graphicsView->setScene(scene);
}