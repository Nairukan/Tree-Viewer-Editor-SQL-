#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <treenode.h>
#include <vector>
#include <QScrollArea>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "cleveredit.h"
#include <stack>
#include "nodeform.h"
#include "paintbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *){
        this->setFocus();
        if (Actives.size()!=0) Actives.top()->repaint();
        for (TreeNode* TN : ViewedNodes){
            if (TN->Editable) TN->Editable=0;
            TN->repaint();
        }
    }
    TreeNode* ActivCatalog=nullptr;
    std::vector<TreeNode*> ViewedNodes;
    TreeNode* GetBySQL(QSqlQuery& query, int T_ID, QWidget* parent);
    QSqlQuery* query=nullptr;
public slots:
    void Func2Click(TreeNode* Sender_ptr);
    void Func1Click(TreeNode* Sender_ptr);
    void FocusOutAnyEdit(CleverEdit* Sender_ptr);
    void FocusINAnyEdit(CleverEdit* Sender_ptr);
private slots:
    void AddClick();
    void DeleteClick();
    void Tick();
    void on_action_triggered();

private:
    PaintButton *PB_Add, *PB_Delete;
    CleverEdit *EditId, *EditName;
    void UpdateNodesWidget();
    void paintEvent(QPaintEvent*);
    void deleteLine(QSqlQuery& query, int T_Id);
    QRect bufferRect;
    QTimer* T;
    QImage PinkIm=QImage("res\\IMG_5253.jpg"); //Предзагруженное фоновое изображение
    QImage Mozaika=QImage("res\\IMG_5252.jpg"); //Предзагруженная текстура кнопки по умолчанию
    QImage DarkMozaika=QImage("res\\IMG_5251.jpg"); //Предзагруженная текстура кнопки при наведении
    int pokaz=0;
    TreeNode* InfoEdit = nullptr;
    std::stack<TreeNode*> Actives;
    QScrollArea * SCROLL_AREA=nullptr;
    NodeForm* ViewedNodesWidget=nullptr;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
