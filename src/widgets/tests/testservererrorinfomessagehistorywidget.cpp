/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "testservererrorinfomessagehistorywidget.h"
#include "servererrorinfohistorymanager.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

TestServerErrorInfoMessageHistoryWidget::TestServerErrorInfoMessageHistoryWidget(QWidget *parent)
    : QWidget{parent}
{
    auto layout = new QFormLayout(this);
    auto accountName = new QLineEdit(this);
    layout->addRow(QStringLiteral("Account Name:"), accountName);

    auto messageStr = new QLineEdit(this);
    layout->addRow(QStringLiteral("Message:"), messageStr);

    auto pushButton = new QPushButton(QStringLiteral("Add Error"), this);
    layout->addWidget(pushButton);

    connect(pushButton, &QPushButton::clicked, this, [accountName, messageStr]() {
        ServerErrorInfo info;
        info.setAccountName(accountName->text());
        info.setMessage(messageStr->text());
        ServerErrorInfoHistoryManager::self()->addServerErrorInfo(info);
    });
}

#include "moc_testservererrorinfomessagehistorywidget.cpp"
