/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "newmessageindicatorgui.h"
#include "room/newmessageindicator.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QStandardPaths>
#include <QTextEdit>
using namespace Qt::Literals::StringLiterals;
NewMessageIndicatorGui::NewMessageIndicatorGui(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    auto textEdit = new QTextEdit(this);
    mainLayout->addWidget(textEdit);

    mNewMessageIndicator = new NewMessageIndicator(this);
    mNewMessageIndicator->move(width() - mNewMessageIndicator->width() - 10, height() - (mNewMessageIndicator->height() * 2));
    mNewMessageIndicator->showNewMessageIndicator(true);
}

NewMessageIndicatorGui::~NewMessageIndicatorGui() = default;

void NewMessageIndicatorGui::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    mNewMessageIndicator->move(width() - mNewMessageIndicator->width() - 10, height() - (mNewMessageIndicator->height() * 2));
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    NewMessageIndicatorGui w;
    w.show();
    return app.exec();
}

#include "moc_newmessageindicatorgui.cpp"
