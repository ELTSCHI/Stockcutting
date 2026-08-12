#include "cuttingplanwidget.h"

#include <QPainter>
#include <QString>
#include <Result.h>

#include <algorithm>
#include <cmath>

const int leftMargin = 160;
const int rightMargin = 90;
const int topMargin = 56;
const int barHeight = 30;
const int rowSpacing = 16;

QColor productColor(int index) {
    return QColor::fromHsv((index * 47 + 20) % 360, 220, 230);
}

QColor stockColor(int index) {
    return QColor::fromHsv((index * 47 + 20) % 360, 70, 245);
}


CuttingPlanWidget::CuttingPlanWidget(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(300);
}

void CuttingPlanWidget::setData(std::vector<Product>& products,
                                std::vector<StockType>& stocks,
                                Result& result) {
    this->products = products;
    this->stocks = stocks;
    this->result.usages.clear();
    for (const auto& usage : result.usages) {
        if (std::round(usage.quantity) > 0.0) {
            this->result.usages.push_back(usage);
        }
    }
    this->result.cost = result.cost;
    this->result.waste = result.waste;
    setMinimumHeight(layoutHeight());
    update();
}

QSize CuttingPlanWidget::sizeHint() const {
    return QSize(700, layoutHeight());
}

int CuttingPlanWidget::layoutHeight() const {
    return topMargin
        + (int) (result.usages.size()) * (barHeight + rowSpacing)
        + rowSpacing;
}

void CuttingPlanWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.fillRect(rect(), palette().base());

    if (result.usages.empty() || products.empty() || stocks.empty()) {
        painter.setPen(palette().placeholderText().color());
        painter.drawText(rect(), Qt::AlignCenter, tr("Nothing calculated yet"));
        return;
    }

    int maxStockLength = 0;
    for (const auto& stock : stocks) {
        maxStockLength = std::max(maxStockLength, stock.length);
    }

    double scale = 1.0;
    if(maxStockLength > 0) {
        scale  = (double) (width() - leftMargin - rightMargin) / maxStockLength;
    }

    drawLegend(painter);

    int y = topMargin;
    for (const auto& usage : result.usages) {
        const Pattern& pattern = usage.pattern;

        StockType& stock = stocks[pattern.stockIndex];
        int usageCount = (int) (usage.quantity);
        const int barX = leftMargin;
        const int barW = (int) (stock.length * scale);

        QString label = stock.name.empty() ? tr("Stock %1").arg(pattern.stockIndex + 1) : QString::fromStdString(stock.name);
        painter.setPen(palette().text().color());
        painter.drawText(QRect(0, y, leftMargin - 10, barHeight), Qt::AlignRight | Qt::AlignVCenter, label);

        painter.drawText(QRect(width() - rightMargin, y, rightMargin - 10, barHeight), Qt::AlignRight | Qt::AlignVCenter, QStringLiteral("x%1").arg(usageCount));

        const QRect stockRect(barX, y, barW, barHeight);
        painter.fillRect(stockRect, stockColor(pattern.stockIndex));

        std::vector<int> pieces;
        for (int i = 0; i < pattern.quantities.size(); i++) {
            int count = (int) (pattern.quantities[i]);
            for (int c = 0; c < count; c++) {
                pieces.push_back(i);
            }
        }

        int x = barX;
        for (int p = 0; p < pieces.size(); p++) {
            Product& product = products[pieces[p]];
            int pieceW = (int) (product.length * scale);
            QRect pieceRect(x, y, pieceW, barHeight);

            painter.fillRect(pieceRect, productColor(pieces[p]));
            painter.setPen(Qt::black);
            painter.drawRect(pieceRect.adjusted(0, 0, 0, 0));

            if (pieceW > 30) {
                painter.drawText(pieceRect, Qt::AlignCenter, QString::number(product.length));
            }

            x += pieceW;
            if (p + 1 < pieces.size()) {
                x += (int) (stock.sawWidth * scale);
            }
        }

        const int wasteW = barW - (x - barX);
        if (wasteW > 0) {
            const QRect wasteRect(x, y, wasteW, barHeight);
            painter.fillRect(wasteRect, QColor(0xdd, 0xdd, 0xdd));
            if (wasteW > 30 && scale > 0.0) {
                painter.setPen(Qt::darkGray);
                Pattern wastePattern = pattern;
                painter.drawText(wasteRect, Qt::AlignCenter, QString::number(wastePattern.getWaste(stocks, products)));
            }
        }

        painter.setPen(Qt::darkGray);
        painter.drawRect(stockRect);

        y += barHeight + rowSpacing;
    }

    painter.setPen(palette().text().color());
    painter.drawText(leftMargin,y + painter.fontMetrics().height(), QString(tr("Waste: %1")).arg(result.waste));
    y+=painter.fontMetrics().height();
    painter.drawText(leftMargin,y + painter.fontMetrics().height(), QString(tr("Cost: %1")).arg(result.cost));


}

void CuttingPlanWidget::drawLegend(QPainter& painter) {
    const int iconSize = 14;
    int x = leftMargin;
    int y = 12;

    auto drawItem = [&](const QString& name, const QColor& color) {
        const int textW = painter.fontMetrics().horizontalAdvance(name);
        const int itemW = iconSize + 6 + textW + 16;
        if (x + itemW > width() - rightMargin) {
            x = leftMargin;
            y += 24;
        }
        painter.fillRect(x, y, iconSize, iconSize, color);
        painter.setPen(Qt::darkGray);
        painter.drawRect(x, y, iconSize, iconSize);
        painter.setPen(palette().text().color());
        painter.drawText(x + iconSize + 6, y + iconSize - 2, name);
        x += itemW;
    };

    for (int i = 0; i < (int) (products.size()); i++) {
        const QString name = products[i].name.empty()
            ? QString(tr("Product %1")).arg(i)
            : QString::fromStdString(products[i].name);
        drawItem(name, productColor(i));
    }
    for (int i = 0; i < (int) (stocks.size()); i++) {
        const QString name = stocks[i].name.empty()
            ? QString(tr("Stock %1")).arg(i)
            : QString::fromStdString(stocks[i].name);
        drawItem(name, stockColor(i));
    }
}
