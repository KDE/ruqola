/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "testservererrorinfomessagehistorywidget.h"
using namespace Qt::Literals::StringLiterals;

#include "servererrorinfohistorymanager.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

TestServerErrorInfoMessageHistoryWidget::TestServerErrorInfoMessageHistoryWidget(QWidget *parent)
    : QWidget{parent}
{
    auto layout = new QFormLayout(this);
    auto accountName = new QLineEdit(this);
    layout->addRow(u"Account Name:"_s, accountName);

    auto messageStr = new QLineEdit(this);
    layout->addRow(u"Message:"_s, messageStr);

    auto pushButton = new QPushButton(u"Add Error"_s, this);
    layout->addWidget(pushButton);

    connect(pushButton, &QPushButton::clicked, this, [accountName, messageStr]() {
        ServerErrorInfo info;
        info.setAccountName(accountName->text());
        info.setMessage(messageStr->text());
        ServerErrorInfoHistoryManager::self()->addServerErrorInfo(info);
    });
}

#include "moc_testservererrorinfomessagehistorywidget.cpp"
