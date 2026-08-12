#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "PatternUsage.h"
#include "Product.h"
#include "StockType.h"

#include <QMainWindow>

#include <vector>

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
    void runSolver();
    void addStockRow();
    void addProductRow();

private:
    Ui::MainWindow *ui;

    void setupStockTable();
    void setupProductTable();
    std::vector<Product> collectProducts();
    std::vector<StockType> collectStocks();
};

#endif // MAINWINDOW_H
