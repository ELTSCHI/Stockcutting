#ifndef CUTTINGPLANWIDGET_H
#define CUTTINGPLANWIDGET_H

#include "PatternUsage.h"
#include "Product.h"
#include "StockType.h"

#include <QWidget>

#include <vector>

class CuttingPlanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CuttingPlanWidget(QWidget *parent = nullptr);

    void setData(std::vector<Product>& products,
                 std::vector<StockType>& stocks,
                 std::vector<PatternUsage>& result);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<Product> products;
    std::vector<StockType> stocks;
    std::vector<PatternUsage> result;

    int layoutHeight() const;
    void drawLegend(QPainter& painter);
};

#endif // CUTTINGPLANWIDGET_H
