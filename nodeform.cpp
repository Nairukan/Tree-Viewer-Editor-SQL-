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


void NodeForm::mousePressEvent(QMouseEvent*){
    this->setFocus();
    this->repaint();
}
