#include "treenode.h"

TreeNode::TreeNode(QWidget *parent, QImage AllTexture, QRect BaseTexture, QRect OneClickTexture, QRect DoubleClickTexture) : QPushButton(parent)
{
    T=new QTimer(this);
    connect(T, &QTimer::timeout, this, &TreeNode::NoTime);
    this->AllTexture=AllTexture;
    this->BaseTexture=BaseTexture;
    *DefaultRect=BaseTexture;
    this->ActiveRect=BaseTexture;
    this->OneClickTexture=OneClickTexture;
    this->DoubleClickTexture=DoubleClickTexture;
    this->IdRect=QRect(0.065*width(), -4, 0.867*width(), 0.38*height()+4);
    this->NameRect=QRect(0.065*width(), 0.307*height(), 0.867*width(), 0.62*height());
    UpdateFontSize();
}

void TreeNode::UpdateFontSize(){
    IdFont=QFont(this->font().family(), 10, 30);
    IdFont.setBold(true);
    QFontMetrics fm(IdFont);
    int sz;
    if (Id!=NULL)
        while (fm.width(QString::number(Id))>IdRect.width() || fm.height()>IdRect.height() && IdFont.weight()!=4){
            IdFont=QFont(IdFont.family(), IdFont.pixelSize(), IdFont.weight()-1);
            fm=QFontMetrics(IdFont);
        }
    else
        while (fm.width("NULL")>IdRect.width() || fm.height()>IdRect.height() && IdFont.weight()!=4){
            IdFont=QFont(IdFont.family(), IdFont.pixelSize(), IdFont.weight()-1);
            fm=QFontMetrics(IdFont);
            sz=IdFont.weight();
        }
    NameFont=QFont(this->font().family(), 10, IdFont.weight()+10);
    NameFont.setBold(true);
    fm=QFontMetrics(NameFont);
    while (fm.width('"'+Name+'"')>NameRect.width() || fm.height()>NameRect.height() && NameFont.weight()!=4){
        NameFont=QFont(NameFont.family(), NameFont.pixelSize(), NameFont.weight()-1);

        fm=QFontMetrics(NameFont);
    }
}

void TreeNode::resizeEvent(QResizeEvent *){
    this->IdRect=QRect(0.065*width(), -4, 0.867*width(), 0.38*height()+4);
    this->NameRect=QRect(0.065*width(), 0.307*height(), 0.867*width(), 0.62*height());
    UpdateFontSize();
}
