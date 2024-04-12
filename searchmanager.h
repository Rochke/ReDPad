#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <QWidget>
#include <QPainter>

class SearchManager : public QWidget
{
public:
    SearchManager(QWidget* = nullptr);
protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // SEARCHMANAGER_H
