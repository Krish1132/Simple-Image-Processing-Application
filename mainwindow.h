#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_uploadBtn_clicked();

    QVector<int> findProperties(QVector<QVector<int>> &imgArray, unsigned int &rows, unsigned int &cols);

    int bfs(unsigned int i, unsigned int j, QVector<QVector<int>> &visited, unsigned int &rows, unsigned int &cols, QVector<QVector<int>> &imgArray);

//    int bfs(QVector<int> & pos, QVector<QVector<int>> &visited, unsigned int &rows, unsigned int &cols, QVector<QVector<int>> &imgArr);

    void mark(int i, int j, unsigned int &rows, unsigned int &cols, QVector<QVector<int>> &imgArray, QImage &image, QVector<QVector<int>> visited);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
