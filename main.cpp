#include "Product.h"
#include "StockType.h"
#include "allpattern.h"
#include "heuristiccolumngeneration.h"
#include "mainwindow.h"

#include <iostream>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

const Product product1{.id = 0, .length = 160, .demand = 6 };
const Product product2{.id = 1, .length = 480, .demand = 7 };
const Product product3{.id = 2, .length = 735, .demand = 4 };
std::vector<Product> products = {product1, product2, product3};

const StockType stock1{.id = 0, .length = 2400, .cost = 1, .availabilty = -1 };
const StockType stock2{.id = 1, .length = 2880, .cost = 1, .availabilty = -1, .sawWidth = 1 };
std::vector<StockType> stocks = {stock1, stock2};

HeuristicColumnGeneration cuttingStockSolver(products, stocks);
AllPattern cuttingStockSolver2(products, stocks);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Stockcutting_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();

    std::vector<PatternUsage> erg = cuttingStockSolver.solve();
    for(auto patternUsage : erg) {
        std::cout << "Pattern: ";
        for(auto patternItem : patternUsage.pattern.quantities) {
            std::cout << patternItem << "|";
        }
        std::cout << "Quantity: " << patternUsage.quantity << "\n";
    }

    erg = cuttingStockSolver2.solve();
    for(auto patternUsage : erg) {
        std::cout << "Pattern: ";
        for(auto patternItem : patternUsage.pattern.quantities) {
            std::cout << patternItem << "|";
        }
        std::cout << "Quantity: " << patternUsage.quantity << "\n";
    }

    return a.exec();
}
