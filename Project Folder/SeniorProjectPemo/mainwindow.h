#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QListView>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QSplitter *splitter = new QSplitter();
    QListView *listview = new QListView;
    QListView *listview2 = new QListView;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
