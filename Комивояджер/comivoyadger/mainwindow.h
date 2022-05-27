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

protected:
    void HideTable();
    void ShowTable();
    void find_min();
    void find_null(int num);
    void paintEvent(QPaintEvent* event);
private slots:
    void on_create_clicked();

    void on_build_clicked();

    void on_calc_clicked();

    void on_Spinbox_valueChanged(int arg1);

    void on_create_pressed();
    void on_create_released();

    void on_build_pressed();

    void on_build_released();

    void on_calc_pressed();

    void on_calc_released();

    void on_clear_clicked();

    void on_fill_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_del_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_clear_pressed();

    void on_clear_released();

    void on_fill_pressed();

    void on_fill_released();

    void on_del_pressed();

    void on_del_released();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
