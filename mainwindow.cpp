#include "mainwindow.h"
#include "allpattern.h"
#include "heuristiccolumngeneration.h"
#include "ui_mainwindow.h"

#include <QHeaderView>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolButton>
#include <limits.h>

namespace {
QString itemText(QTableWidget* table, int row, int column) {
    QTableWidgetItem* item = table->item(row, column);
    return item ? item->text().trimmed() : QString();
}

int spinValue(QTableWidget* table, int row, int column) {
    QSpinBox* spin = qobject_cast<QSpinBox*>(table->cellWidget(row, column));
    return spin ? spin->value() : 0;
}

void setCell(QTableWidget* table, int row, int column, const QString& text) {
    table->setItem(row, column, new QTableWidgetItem(text));
}

void setSpin(QTableWidget* table, int row, int column, int value) {
    QSpinBox* spin = qobject_cast<QSpinBox*>(table->cellWidget(row, column));
    if (!spin) {
        return;
    }
    spin->setValue(value);
}

void addRow(QTableWidget* table, const QList<int>& textColumns, const QList<int>& spinColumns) {
    const int row = table->rowCount();
    table->insertRow(row);
    for (int col : textColumns) {
        table->setItem(row, col, new QTableWidgetItem);
    }
    for (int col : spinColumns) {
        QSpinBox* spin = new QSpinBox;
        spin->setRange(0, INT_MAX);
        table->setCellWidget(row, col, spin);
    }
}

void setUnlimitedSpin(QTableWidget* table, int row, int column,
             const QString& unlimitedString = QString()) {
    QSpinBox* spin = qobject_cast<QSpinBox*>(table->cellWidget(row, column));
    if (!spin) {
        return;
    }
    spin->setRange(-1, INT_MAX);
    spin->setValue(-1);
    if (!unlimitedString.isEmpty()) {
        spin->setSpecialValueText(unlimitedString);
    }
}
} // namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle(tr("Stock Cutting"));
    ui->sawWidthLabel->setText(tr("Saw Width"));
    ui->stockGroup->setTitle(tr("Stock"));
    ui->productGroup->setTitle(tr("Products"));
    ui->modelGroup->setTitle(tr("Calculation Model"));
    ui->radioAll->setText(tr("All Cutting Patterns"));
    ui->radioHeuristic->setText(tr("Heuristic Column Generation"));
    ui->applyButton->setText(tr("Calculate"));

    setupStockTable();
    setupProductTable();

    connect(ui->applyButton, &QPushButton::clicked, this, &MainWindow::runSolver);
    connect(ui->addStockButton, &QToolButton::clicked, this, &MainWindow::addStockRow);
    connect(ui->addProductButton, &QToolButton::clicked, this, &MainWindow::addProductRow);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupStockTable() {
    ui->stockTable->setColumnCount(4);
    ui->stockTable->setHorizontalHeaderLabels({tr("Name"), tr("Length"), tr("Amount"), tr("Cost")});
    ui->stockTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stockTable->verticalHeader()->setVisible(false);

    addStockRow();
}

void MainWindow::setupProductTable() {
    ui->productTable->setColumnCount(3);
    ui->productTable->setHorizontalHeaderLabels({tr("Name"), tr("Length"), tr("Amount")});
    ui->productTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->productTable->verticalHeader()->setVisible(false);

    addProductRow();
}

void MainWindow::addStockRow() {
    addRow(ui->stockTable, {0}, {1, 2, 3});
    int row = ui->stockTable->rowCount() - 1;
    setCell(ui->stockTable, row, 0, QString(tr("Stock %1")).arg(row));
    setUnlimitedSpin(ui->stockTable, row, 2, tr("unlimited"));
    setSpin(ui->stockTable, row, 1, 700);
    setSpin(ui->stockTable, row, 3, 1);
}

void MainWindow::addProductRow() {
    addRow(ui->productTable, {0}, {1, 2});
    int row = ui->productTable->rowCount() - 1;
    setCell(ui->productTable, row, 0, QString(tr("Product %1")).arg(row));
    setSpin(ui->productTable, row, 1, 100);
    setSpin(ui->productTable, row, 2, 10);
}

void MainWindow::runSolver() {
    std::vector<Product> products = collectProducts();
    std::vector<StockType> stocks = collectStocks();

    Result result;
    if (ui->radioAll->isChecked()) {
        AllPattern solver(products, stocks);
        result = solver.solve();
    } else {
        HeuristicColumnGeneration solver(products, stocks);
        result = solver.solve();
    }

    ui->cuttingPlanWidget->setData(products, stocks, result);
    statusBar()->showMessage(tr("Calculation has terminated"), 3000);
}

std::vector<Product> MainWindow::collectProducts() {
    std::vector<Product> products;
    for (int row = 0; row < ui->productTable->rowCount(); row++) {
        Product product;
        product.name = itemText(ui->productTable, row, 0).toStdString();
        product.length = spinValue(ui->productTable, row, 1);
        product.demand = spinValue(ui->productTable, row, 2);
        if (product.length <= 0) {
            continue;
        }
        products.push_back(product);
    }
    return products;
}

std::vector<StockType> MainWindow::collectStocks() {
    std::vector<StockType> stocks;
    for (int row = 0; row < ui->stockTable->rowCount(); row++) {
        StockType stock;
        stock.name = itemText(ui->stockTable, row, 0).toStdString();
        stock.length = spinValue(ui->stockTable, row, 1);
        if (stock.length <= 0) {
            continue;
        }
        stock.availabilty = spinValue(ui->stockTable, row, 2);
        stock.cost = spinValue(ui->stockTable, row, 3);
        stock.sawWidth = ui->sawWidthSpin->value();
        stocks.push_back(stock);
    }
    return stocks;
}
