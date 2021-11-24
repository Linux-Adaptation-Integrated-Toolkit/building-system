#include "widget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

Widget::~Widget()
{
}

void Widget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    QPushButton *testBtn = new QPushButton("test");
    connect(testBtn,SIGNAL(clicked(bool)),this,SLOT(slot_testBtn_clicked()));
    mainLayout->addWidget(testBtn);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

void Widget::slot_testBtn_clicked()
{
    qDebug()<<tr("单击test按钮");
}