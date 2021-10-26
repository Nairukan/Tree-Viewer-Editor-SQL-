#include "nodeform.h"
#include "ui_nodeform.h"

NodeForm::NodeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodeForm)
{
    ui->setupUi(this);
}

NodeForm::~NodeForm()
{
    delete ui;
}


void NodeForm::paintEvent(QPaintEvent*){
    if (this->children().empty()){
        QPainter p(this);
        p.setPen(Qt::black);
        QFont f(p.font().family(), 30, 60);
        QFontMetrics fm(f);
        while (fm.width("<Нет элементов>")>this->width()*0.7 && fm.height()>this->height()*0.3) {
            f.setWeight(f.weight()-1);
            fm=QFontMetrics(f);
        }
        p.setFont(f);
        p.drawText(QRect(0,0, width(), height()), Qt::AlignCenter ,"<Нет элементов>");
    }
}

void NodeForm::mousePressEvent(QMouseEvent*){
    this->setFocus();
    this->repaint();
}
