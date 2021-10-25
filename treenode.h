#ifndef TREEORD_H
#define TREEORD_H

#define DEBAG

#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include <QImage>
#include <QFontMetrics>

#ifdef DEBAG
    #include <QMessageBox>
#endif

class TreeNode : public QPushButton
{
    Q_OBJECT
public:

    TreeNode(QWidget *parent = nullptr, QImage AllTexture=QImage(), QRect BaseTexture=QRect(), QRect OneClickTexture=QRect(), QRect DoubleClickTexture=QRect());

    void mousePressEvent(QMouseEvent *){
        this->setFocus();
        kolPressFor300ms++;
        if (!T->isActive()){
            T->start(200);
        }
    }

    ~TreeNode(){
        delete T;
    }

    QRect* DefaultRect=new QRect;

    void UpdateFontSize();

    void paintEvent(QPaintEvent * event){
        QPainter p(this);
        if (!AllTexture.isNull() && !ActiveRect.isNull()){
            if (ActiveRect==OneClickTexture) Com=1;
            else if (ActiveRect==DoubleClickTexture) Com=2;
            else Com=0;
            p.drawImage(QRect(0,0, width(), height()), AllTexture, ActiveRect);
        }
        p.setFont(IdFont);
        if (this->Id==NULL)
            p.drawText(this->IdRect, Qt::AlignCenter, "NULL");
        else
            p.drawText(this->IdRect, Qt::AlignCenter, QString::number(this->Id));
        p.setFont(NameFont);
        p.drawText(this->NameRect, Qt::AlignCenter, '"'+this->Name+'"');
    }

    void focusOutEvent(QFocusEvent *e){
        T->stop();
        kolPressFor300ms=0;
        ActiveRect=*DefaultRect;
        repaint();
    }

    void setName(QString Name){
        this->Name=Name;
    }

    void setId(int Id){
        this->Id=Id;
    }

    void setT_Id(int T_Id){
        this->TrueId=T_Id;
    }

    int getT_Id(){
        return this->TrueId;
    }

    int getId(){
        return Id;
    }

    QString getName(){
        return Name;
    }

    QRect BaseTexture, OneClickTexture, DoubleClickTexture;
    QRect ActiveRect;
    int Com=0;
    int Editable=0;
private slots:
    void NoTime(){
      if (kolPressFor300ms>1){
          ActiveRect=DoubleClickTexture;
          repaint();
          emit DoubleClick();
          emit DblClickGlobal(this);
      }
      else if (kolPressFor300ms==1){
          ActiveRect=OneClickTexture;
          repaint();
          emit SingleClick();
          emit SnglClickGlobal(this);
      }
      else T->stop();
      kolPressFor300ms=0;
    }



signals:
    void DblClickGlobal(TreeNode* Sender_ptr);
    void SnglClickGlobal(TreeNode* Sender_ptr);
    void SingleClick();
    void DoubleClick();
private:
    void resizeEvent(QResizeEvent *);
    QRect IdRect, NameRect;
    QImage AllTexture;
    int TrueId;
    QFont IdFont, NameFont;
    int Id=NULL;
    QString Name="";
    QTimer * T;
    unsigned int kolPressFor300ms=0;
};

#endif // TREEORD_H
