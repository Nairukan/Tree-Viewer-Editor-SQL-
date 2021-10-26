#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

int BinaryFind(QSqlQuery& query, int T_Id, int f, int l){
    if (f<0 || l<0) return -1;
    if (l-f==0){
        query.seek(l);
        if (query.value(0)!=T_Id) return -1;
        return l;
    }
    query.seek(f+(l-f)/2);
    int T_ID_middleElem=query.value(0).toInt();
    if (T_Id<=T_ID_middleElem) return BinaryFind(query, T_Id, f, f+(l-f)/2);
    else  return BinaryFind(query, T_Id, f+(l-f+1)/2, l);
}

QSqlDatabase db;

void addNewLine(QSqlQuery& query, int Id, QString Name, int Parent_T_ID=0){
    int lineParent=-1;

    query.exec("SELECT * FROM Nodes;");
    int qsz=0;
    if (query.last())
        qsz=query.at()+1;
    if (Parent_T_ID!=-1 && qsz!=0)
        lineParent=BinaryFind(query, Parent_T_ID, 0, qsz-1);
    int NewT_Id=0;
    if (qsz){
        query.last();
        NewT_Id=query.value(0).toInt()+1;
    }
    QString Err=query.lastError().text();
    if (lineParent!=-1){
        query.seek(lineParent);
        QString Sons=query.value(3).toString();
        QStringList list=Sons.split('_');
        if (Sons.length()!=0) Sons+="_";
        Sons+=QString::number(NewT_Id);
        bool some=query.exec("INSERT INTO Nodes VALUES("+QString::number(NewT_Id)+", "+QString::number(Id)+", '"+Name+"', '', "+QString::number(Parent_T_ID)+");");
        some=query.exec("UPDATE Nodes SET sons = '"+Sons+"' WHERE T_Id = "+QString::number(Parent_T_ID)+";");
        int a=4;
    }else{
        bool some=query.exec("INSERT INTO Nodes VALUES("+QString::number(NewT_Id)+", NULL, '"+Name+"', '', NULL);");
        int a=4;
    }
    query.exec("SELECT * FROM Nodes;");

}

void deleteSons(QSqlQuery& query, int T_Id){
    int qsz=0;
    if (query.last())
        qsz=query.at()+1;
    query.seek(BinaryFind(query, T_Id, 0, qsz-1));
    QString Sons=query.value(3).toString();
    QStringList list=Sons.split('_');
    for (auto elem : list){
        if (elem!=""){
            int e=elem.toInt();
            deleteSons(query, e);
            bool cool=query.exec("SELECT * FROM Nodes ;");
            cool=query.exec("DELETE FROM Nodes WHERE T_ID = "+QString::number(e))+" LIMIT 1 ;";
            cool=query.exec("SELECT * FROM Nodes ;");
        }
    }
    query.exec("SELECT * FROM Nodes ;");
    query.exec("DELETE FROM Nodes WHERE T_ID = "+QString::number(T_Id))+" LIMIT 1 ;";
    query.exec("SELECT * FROM Nodes ;");
}



void MainWindow::deleteLine(QSqlQuery& query, int T_Id){
    query.seek(T_Id);
    int fath=query.value(4).toInt();
    if (true){
        int qsz=0;
        if (query.last())
            qsz=query.at()+1;
        query.seek(BinaryFind(query, fath, 0, qsz-1));
        QString brothers=query.value(3).toString();
        QStringList list=brothers.split('_');
        brothers="";
        for (QString t : list){
            if (t!=QString::number(T_Id)){
                if (brothers!="")
                    brothers+="_";
                brothers+=t;
            }
        }
        query.exec("SELECT * FROM Nodes ;");
        bool com=this->query->exec("UPDATE Nodes SET sons = '" + brothers + "' WHERE T_Id = "+QString::number(fath)+" ;");
        query.exec("SELECT * FROM Nodes ;");
    }
    deleteSons(query, T_Id);

}

void MainWindow::Func2Click(TreeNode* Sender_ptr){
    if (Actives.size()==0 || Actives.top()!=Sender_ptr){
        InfoEdit=nullptr;
        for (TreeNode* node_ptr : ViewedNodes){
            if (node_ptr!=Sender_ptr) delete node_ptr;
        }
        ViewedNodes.clear();
        if (Actives.size()!=0) Actives.top()->hide();
        Actives.push(Sender_ptr);
        Sender_ptr->setParent(this);
        Sender_ptr->move(0.41*this->width(), 0.047*this->height());
        Sender_ptr->show();
        Sender_ptr->ActiveRect=Sender_ptr->DoubleClickTexture;
        Sender_ptr->DefaultRect=Sender_ptr->DoubleClickTexture;
        Sender_ptr->repaint();
        query->exec("SELECT * FROM Nodes;");
        int qsz=0;
        if (query->last())
            qsz=query->at()+1;
        int row = BinaryFind(*query, Sender_ptr->getT_Id(), 0, qsz-1);
        query->seek(row);
        QString Sons=query->value(3).toString();
        QStringList SL=Sons.split('_');
        for (QString str : SL){
            if (str!=""){
                for (int i=0; i<str.length(); i++)
                    if (!str[i].isDigit()) goto NoThis1;
                TreeNode* Node=GetBySQL(*query, str.toInt(), this);
                if (Node==nullptr) goto NoThis1;
                ViewedNodes.push_back(Node);
            }
            NoThis1:{}
        }
        UpdateNodesWidget();
    }else if (Actives.size()!=0 && Sender_ptr==Actives.top() && Actives.top()->getT_Id()!=0){
        InfoEdit=nullptr;
        for (TreeNode* node_ptr : ViewedNodes){
            delete node_ptr;
        }
        ViewedNodes.clear();
        Actives.pop();
        Actives.top()->show();
        int qsz=0;
        if (query->last())
            qsz=query->at()+1;
        int row = BinaryFind(*query, Actives.top()->getT_Id(), 0, qsz-1);
        query->seek(row);
        QString Sons=query->value(3).toString();
        QStringList SL=Sons.split('_');
        for (QString str : SL){
            if (str!=""){
                for (int i=0; i<str.length(); i++)
                    if (!str[i].isDigit()) goto NoThis2;
                TreeNode* Node=GetBySQL(*query, str.toInt(), this);
                if (Node==nullptr) goto NoThis2;
                ViewedNodes.push_back(Node);
            }
            NoThis2:{}
        }
        UpdateNodesWidget();
        delete Sender_ptr;
    }
}

void MainWindow::Func1Click(TreeNode* Sender_ptr){
    if (InfoEdit!=nullptr){
            InfoEdit->ActiveRect=InfoEdit->DefaultRect;
            InfoEdit->repaint();
            InfoEdit=nullptr;
        if (T->isActive()) T->stop();
    }
    this->EditId->setText(QString::number(Sender_ptr->getId()));
    this->EditName->setText(Sender_ptr->getName());
    this->EditId->setEnabled(true);
    this->EditName->setEnabled(true);
    InfoEdit=Sender_ptr;
    InfoEdit->repaint();

    //for iniz
    EditId->setFocus();
    InfoEdit->repaint();
    EditName->setFocus();

    InfoEdit->ActiveRect=InfoEdit->OneClickTexture;
    InfoEdit->repaint();
    EditId->setFocus();
}


void MainWindow::AddClick(){
    if (InfoEdit!=nullptr && Actives.size()!=0 && Actives.top()==InfoEdit){\
        bool com=query->exec("SELECT * FROM Nodes ;");
        addNewLine(*query, NULL, "NULL NAME", InfoEdit->getT_Id());
        query->exec("SELECT * FROM Nodes ;");
        query->last();
        int T_Id=query->value(0).toInt();
        TreeNode* ND = GetBySQL(*query, T_Id, this);
        ViewedNodes.push_back(ND);
        ND->setParent(ViewedNodesWidget);
        int i=ViewedNodes.size()-1;
        ND->move(15+(211+22)*(i%5), 31+(78+15)*(i/5));
        ND->repaint();
        ND->setHidden(false);
        UpdateNodesWidget();
    }
}

void MainWindow::DeleteClick(){
    if (InfoEdit!=nullptr && Actives.size()!=0 && Actives.top()==InfoEdit && Actives.top()->getT_Id()!=0){
        if (T->isActive()) T->stop();
        query->exec("SELECT * FROM Nodes ;");
        deleteLine(*query, InfoEdit->getT_Id());
        for (TreeNode* node_ptr : ViewedNodes){
            delete node_ptr;
        }
        ViewedNodes.clear();
        Actives.pop();
        Actives.top()->show();
        query->exec("SELECT * FROM Nodes;");
        int qsz=0;
        if (query->last())
            qsz=query->at()+1;
        int row = BinaryFind(*query, Actives.top()->getT_Id(), 0, qsz-1);
        query->seek(row);
        QString Sons=query->value(3).toString();
        QStringList SL=Sons.split('_');
        for (QString str : SL){
            if (str!=""){
                for (int i=0; i<str.length(); i++)
                    if (!str[i].isDigit()) goto NoThis2;
                TreeNode* Node=GetBySQL(*query, str.toInt(), this);
                if (Node==nullptr) goto NoThis2;
                ViewedNodes.push_back(Node);
            }
            NoThis2:{}
        }
        UpdateNodesWidget();
        delete InfoEdit;
    }
}

void MainWindow::FocusINAnyEdit(CleverEdit* Sender_ptr){
    if (InfoEdit!=nullptr){
        InfoEdit->repaint();
        pokaz++;
    }
}

void MainWindow::Tick(){
    if (!pokaz){
        if (InfoEdit!=nullptr){
            InfoEdit->ActiveRect=InfoEdit->DefaultRect;
            InfoEdit->repaint();
            InfoEdit=nullptr;
        }
        this->EditId->setText("");
        this->EditName->setText("");
        this->EditId->setEnabled(false);
        this->EditName->setEnabled(false);
        EditId->setFocus();
        EditName->setFocus();
        this->setFocus();
    }
    T->stop();
}

void MainWindow::FocusOutAnyEdit(CleverEdit* Sender_ptr){
    if (InfoEdit!=nullptr){
        if (Sender_ptr==this->EditId){
            if (Sender_ptr->GetState()==QValidator::Acceptable){
                query->exec("SELECT * FROM Nodes;");
                query->exec("UPDATE Nodes SET Id = " + Sender_ptr->text() + " WHERE T_Id = "+QString::number(InfoEdit->getT_Id())+" ;");

                query->exec("SELECT * FROM Nodes;");
                InfoEdit->setId(Sender_ptr->text().toInt());

            }
            InfoEdit->repaint();
        }else if (Sender_ptr==this->EditName){
            if (Sender_ptr->GetState()==QValidator::Acceptable){
                query->exec("SELECT * FROM Nodes;");
                query->exec("UPDATE Nodes SET Name = '" + Sender_ptr->text() + "' WHERE T_Id = "+QString::number(InfoEdit->getT_Id())+" ;");

                query->exec("SELECT * FROM Nodes;");
                InfoEdit->setName(Sender_ptr->text());
            }
            InfoEdit->repaint();
        }
        pokaz--;
        T->start(300);
    }
}

bool ValidForIdEdit(QString a){
    QRegExp rx("^\\d+$");
    return rx.indexIn(a)+1;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName ("localhost");
    db.setDatabaseName("treeNodes.sqlite");
    db.setUserName("Admin");
    db.setPassword("Password");
    bool ope=db.open();
    QString Err=db.lastError().text();
    this->query = new QSqlQuery(db);
         if (query->exec("CREATE TABLE Nodes "
                    "(T_Id int, id int, "
                    "name varchar(20) NOT NULL, sons bigtext, parent_T_Id int);")) //если не находит базу то создаём новую
         {
             addNewLine(*query, NULL, "NULL_CATEGORY", -1);
             addNewLine(*query, 1, "1-st Cat", 0);
             addNewLine(*query, 2, "2-nd Cat", 0);
             addNewLine(*query, 3, "3-rd Cat", 0);
             addNewLine(*query, 4, "3-1-st Cat", 3);
             addNewLine(*query, 5, "3-2-st Cat", 3);
             addNewLine(*query, 6, "1-1-st Cat", 1);
             addNewLine(*query, 7, "2-2-st Cat", 2);
         }

    //query->exec("SELECT * FROM Nodes ;");
    //while (query->next())
    //    qDebug() << query->value(0).toInt() << query->value(1).toInt() <<  query->value(2).toString() <<  query->value(3).toString() <<  query->value(4).toInt() << "\n";
    ui->setupUi(this);
        this->setWindowTitle("TreeWork for Adept");
        this->setFixedSize(1250, 720);

        this->ViewedNodes.push_back(GetBySQL(*query, 0, this));
        connect(ViewedNodes[0], &TreeNode::DblClickGlobal, this, &MainWindow::Func2Click);
        connect(ViewedNodes[0], &TreeNode::SnglClickGlobal, this, &MainWindow::Func1Click);
        ViewedNodes[0]->resize(211, 78);

        SCROLL_AREA=new QScrollArea(this);
        SCROLL_AREA->setGeometry(0.024*this->width(), 0.175*this->height(), 0.96*this->width(), 0.59*this->height());
        ViewedNodesWidget=new NodeForm(SCROLL_AREA);
        ViewedNodesWidget->setGeometry(0,0, SCROLL_AREA->width()-20, 2000);
        SCROLL_AREA->setWidget(ViewedNodesWidget);

        PB_Add=new PaintButton(this, this->Mozaika, this->DarkMozaika);
        PB_Add->setText("+ADD");
        PB_Add->setFont(QFont(PB_Add->font().family(), 30, 30));
        PB_Add->setGeometry(0.024*this->width(), 0.80*this->height(), 0.2*this->width(), 0.121*this->height());
        connect(PB_Add, &QPushButton::clicked, this, &MainWindow::AddClick);

        PB_Delete=new PaintButton(this, this->Mozaika, this->DarkMozaika);
        PB_Delete->setText("-DELETE");
        PB_Delete->setFont(QFont(PB_Delete->font().family(), 30, 30));
        PB_Delete->setGeometry(0.765*this->width(), 0.80*this->height(), 0.22*this->width(), 0.121*this->height());
        connect(PB_Delete, &QPushButton::clicked, this, &MainWindow::DeleteClick);

        EditId=new CleverEdit(this, "Введите новое значение поля <id>", CleverEdit::Normal, &ValidForIdEdit);
        EditId->setGeometry(0.247*this->width(), 0.77*this->height(), 0.487*this->width(), 0.065*this->height());
        EditId->setEnabled(false);
        connect(EditId, &CleverEdit::FocusOutGlobal, this, &MainWindow::FocusOutAnyEdit);
        connect(EditId, &CleverEdit::FocusInGlobal, this, &MainWindow::FocusINAnyEdit);

        EditName=new CleverEdit(this, "Введите новое значение поля <Name>", CleverEdit::Normal, &CleverEdit::NoEmptyValidFunc, true);
        EditName->setGeometry(0.247*this->width(), 0.84*this->height(), 0.487*this->width(), 0.101*this->height());
        EditName->setEnabled(false);
        connect(EditName, &CleverEdit::FocusOutGlobal, this, &MainWindow::FocusOutAnyEdit);
        connect(EditName, &CleverEdit::FocusInGlobal, this, &MainWindow::FocusINAnyEdit);

        T=new QTimer(this);
        connect(T, &QTimer::timeout, this, &MainWindow::Tick);
        UpdateNodesWidget();
}

void MainWindow::paintEvent(QPaintEvent*){
}

void MainWindow::UpdateNodesWidget(){
    for (int i=0; i<ViewedNodes.size(); i++){
        ViewedNodes[i]->setParent(ViewedNodesWidget);
        ViewedNodes[i]->move(15+(211+22)*(i%5), 31+(78+15)*(i/5));
        ViewedNodes[i]->show();
        ViewedNodes[i]->setHidden(false);
        ViewedNodes[i]->repaint();
    }
    if (ViewedNodes.size()==0)
        ViewedNodesWidget->resize(ViewedNodesWidget->width(), SCROLL_AREA->height()-5);
    else
        ViewedNodesWidget->resize(ViewedNodesWidget->width(), std::max(ViewedNodes[ViewedNodes.size()-1]->height()+ViewedNodes[ViewedNodes.size()-1]->y()+12, SCROLL_AREA->height()-5));
    ViewedNodesWidget->repaint();
}

TreeNode* MainWindow::GetBySQL(QSqlQuery& query, int T_ID, QWidget* parent){
    query.exec("SELECT * FROM Nodes;");
    int qsz=0;
    if (query.last())
        qsz=query.at()+1;
    int row=BinaryFind(query, T_ID, 0, qsz-1);
    if (row==-1) return nullptr;
    query.seek(row);
    TreeNode* NewNode = new TreeNode(parent, QImage("RES.png"), QRect(420,0,211,78), QRect(210,0,211,78), QRect(0,0,211,78));
    connect(NewNode, &TreeNode::DblClickGlobal, this, &MainWindow::Func2Click);
    connect(NewNode, &TreeNode::SnglClickGlobal, this, &MainWindow::Func1Click);
    NewNode->setT_Id(T_ID);
    NewNode->setId(query.value(1).toInt());
    NewNode->setName(query.value(2).toString());
    NewNode->resize(211, 78);
    return NewNode;

}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}



void MainWindow::on_action_triggered()
{
    QMessageBox::information(this, "Инструкция", QString("По центру в окне представлены узлы доступные для входа, для входа в узел выберите его в окне и нажмите 2 раза мышкой\n")+
                                                 QString("Над окном находится текущий узел, для выхода(возврата в предыдущий узел) нажмите два раза на активный узел\n")+
                                                 QString("Редактирования содержимого узлов доступно в любом положении узла (Сверху или в окне), для этого необходимо один раз нажать на узел")+
                                                 QString("после чего будет предложено ввести новые значения для полей в это время узел подсветиться голубым, после ввода, при условии что вве-")+
                                                 QString("дённые данные соответсвуют форматам для ввода, они автоматически будут занесены в узлы. Так же в режиме редактирования есть")+
                                                 QString("ещё две команды доступные только при выборе текущего узла(Узел над окном)-Добавление нового узла отходящего от текущего и Удаление")+
                                                 QString("текущего узла со всеми узлами находящимися в нём"));
}
