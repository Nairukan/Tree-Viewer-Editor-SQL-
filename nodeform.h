#ifndef NODEFORM_H
#define NODEFORM_H

#include <QWidget>
#include <QPainter>
#include <QFontMetrics>

namespace Ui {
class NodeForm;
}

class NodeForm : public QWidget
{
    Q_OBJECT

public:
    explicit NodeForm(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent*);
    ~NodeForm();
    void paintEvent(QPaintEvent*);

private:
    Ui::NodeForm *ui;
};

#endif // NODEFORM_H
