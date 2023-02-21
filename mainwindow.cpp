#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QQueue>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_uploadBtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), QDir::homePath(), tr("Images(*.jpg *.png"));
    if(!filename.isEmpty())
    {
        QMessageBox::information(this, "Image status", "Image successfully loaded!", QMessageBox::Button::Ok);
        QImage img(filename);
        QPixmap pix = QPixmap::fromImage(img);
        ui->queryImage->setPixmap(pix.scaled(ui->queryImage->width(), ui->queryImage->height(), Qt::KeepAspectRatio));

        unsigned int numBlack = 0;
        unsigned int c = img.width();
        unsigned int r = img.height();

        ui->progressBar->setRange(0, r);

        QVector<QVector<int>> imgArray(r, QVector(c, 0));

        for(unsigned int i = 0; i < r; i++)
        {
            for(unsigned int j = 0; j < c; j++)
            {
                QColor currPixel(img.pixel( j, i ));
                unsigned int a = currPixel.alpha();
                unsigned int r = currPixel.red();
                unsigned int g = currPixel.green();
                unsigned int b = currPixel.blue();

                if(r + g + b < 20 && a > 240)
                {
                    imgArray[i][j] = 1;
                    numBlack += 1;
                }
            }
        }

        QString filename = "C:/Users/krish/OneDrive/Documents/Project/Output.txt";
        QFile file(filename);
        if(file.open(QFile::ReadWrite | QFile::Text))
        {
            QTextStream out(&file);
            for(unsigned int i = 0; i < r; i++)
            {
                for(unsigned int j = 0; j < c; j++)
                {
                    out << imgArray[i][j];
                }
                out << Qt::endl;
            }
        }
        ui->heightLabel->setText("Height: " + QString::number(r) + " px");
        ui->widthLabel->setText("Width: " + QString::number(c) + " px");
        float occupiedArea = ((float)numBlack / (float)(r * c)) * 100;
        ui->occupancy->setText(QString::number(occupiedArea) + " %");
        QVector<int> res = findProperties(imgArray, r, c);
        ui->numBlobs->setText(QString::number(res[0]));
        ui->maxArea->setText(QString::number(res[1]));

//        QVector<QVector<int>> visited(r, QVector(c, 0));
//        mark(res[2], res[3], r, c, imgArray, img, visited);
//        ui->queryImage->setPixmap(QPixmap::fromImage(img));
    }

}

QVector<int> MainWindow::findProperties(QVector<QVector<int>> &imgArray, unsigned int &rows, unsigned int &cols)
{
    int numIslands = 0;
    int maxArea = 0;
    int area = 0;

    unsigned int max_i = 0;
    unsigned int max_j = 0;

    QVector<QVector<int>> visited(rows, QVector<int>(cols, 0));

    for (unsigned int i = 0; i < rows; i++){
        for (unsigned int j = 0; j < cols; j++){
            area = 0;
            if (imgArray[i][j] == 1 && visited[i][j] == 0){
                area = this->bfs(i, j, visited, rows, cols, imgArray);
                if (area > maxArea){
                    maxArea = area;
                    max_i = i;
                    max_j = j;
                }
                numIslands += 1;
            }
        }
        ui->progressBar->setValue(i);
    }
    QVector<int> results{numIslands, maxArea, (int)max_i, (int)max_j};
    return results;
}

int MainWindow::bfs(unsigned int i, unsigned int j, QVector<QVector<int>> &visited, unsigned int &rows, unsigned int &cols, QVector<QVector<int>> &imgArray){
    if(i < 0 || i >= (int)rows || j < 0 || j >= (int)cols || visited[i][j] == 1 || imgArray[i][j] == 0) {
        return 0;
    }

    visited[i][j] = 1;
    int area = 1;
    area += bfs(i + 1, j, visited, rows, cols, imgArray) + bfs(i - 1, j, visited, rows, cols, imgArray) + bfs(i, j + 1, visited, rows, cols, imgArray) + bfs(i, j - 1, visited, rows, cols, imgArray);
    return area;
}

void MainWindow::mark(int i, int j, unsigned int &rows, unsigned int &cols, QVector<QVector<int>> &imgArray, QImage &image, QVector<QVector<int>> visited)
{
    if(i < 0 || i >= (int)rows || j < 0 || j >= (int)cols || visited[i][j] == 1 || imgArray[i][j] == 0) {
        return;
    }

    QColor pixel(image.pixel(j, i));
    pixel.setRed(255);
    pixel.setGreen(10);
    pixel.setBlue(100);
    visited[i][j] = 1;
    mark(i + 1, j, rows, cols, imgArray, image, visited);
    mark(i - 1, j, rows, cols, imgArray, image, visited);
    mark(i, j + 1, rows, cols, imgArray, image, visited);
    mark(i, j - 1, rows, cols, imgArray, image, visited);
}

//QVector<int> MainWindow::findProperties(QVector<QVector<int>> &imgArr, unsigned int &rows, unsigned int &cols)
//{
//    int numIslands = 0;
//    int maxArea = 0;
//    int area = 0;
//    QVector<QVector<int>> visited(rows, QVector<int>(cols, 0));

//    for(unsigned int i = 0; i < rows; i++)
//    {
//        for(unsigned int j = 0; j < cols; j++)
//        {
//            if(imgArr[i][j] == 1 && visited[i][j] == 0)
//            {
//                QVector<int> pos{(int)i, (int)j};
//                area = this->bfs(pos, visited, rows, cols, imgArr);
//                if(area > maxArea)
//                {
//                    maxArea = area;
//                }
//                numIslands += 1;
//                ui->progressBar->setValue(j * i);
//            }
//        }
//    }
//    QVector<int> res{numIslands, maxArea};
//    return res;
//}

//int MainWindow::bfs(QVector<int> &pos, QVector<QVector<int>> &visited, unsigned int &rows, unsigned int &cols, QVector<QVector<int>> &imgArr)
//{
//    QVector<int> u{-1, 0};
//    QVector<int> d{1, 0};
//    QVector<int> l{0, -1};
//    QVector<int> r{0, 1};

//    int islandArea = 0;

//    QQueue<QVector<int>> not_explored;
//    not_explored.enqueue(pos);

//    while( !(not_explored.isEmpty()) )
//    {
//        QVector<QVector<int>> moves {{pos[0] + u[0], pos[1] + u[1]}, {pos[0] + d[0], pos[1] + d[1]},
//                                     {pos[0] + l[0], pos[1] + l[1]}, {pos[0] + r[0], pos[1] + r[1]}};
//        for(QVector<int> move : moves)
//        {
//            if(move[0] < 0 || move[0] > (int)rows - 1 ||
//               move[1] < 0 || move[1] > (int)cols - 1 ||
//               visited[move[0]][move[1]] == 1 || imgArr[move[0]][move[1]] == 0)
//            {
//                continue;
//            }
//            not_explored.enqueue(move);
//        }
//        visited[pos[0]][pos[1]] = 1;
//        islandArea += 1;
//        not_explored.dequeue();

//        if( !(not_explored.isEmpty()) )
//        {
//            pos = not_explored.head();
//        }
//    }
//    return islandArea;
//}
