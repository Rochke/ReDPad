#include "searchmanager.h"

SearchManager::SearchManager(QWidget *parent) {
    Q_UNUSED(parent);
}

void SearchManager::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect(10, 10, 100, 100);
    qreal radius = 10.0;

    painter.drawRoundedRect(rect, radius, radius);
}
