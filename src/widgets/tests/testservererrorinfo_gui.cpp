/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfohistory/servererrorinfomessagehistorydialog.h"
#include "servererrorinfohistorymanager.h"
#include "testservererrorinfomessagehistorywidget.h"
#include <QApplication>
#include <QCommandLineParser>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);
    auto w = new ServerErrorInfoMessageHistoryDialog();
    w->resize(800, 600);
    w->show();

    auto configServerInfo = new TestServerErrorInfoMessageHistoryWidget();
    configServerInfo->resize(600, 300);
    configServerInfo->show();

    QStringList lst;
    for (int i = 0; i < 10; ++i) {
        ServerErrorInfo info;
        const QString accountName = QStringLiteral("account %1").arg(QString::number(i));
        info.setAccountName(accountName);
        const QString str = QStringLiteral("Message: %1").arg(QString::number(i));
        info.setMessage(str);
        ServerErrorInfoHistoryManager::self()->addServerErrorInfo(info);
        lst << accountName;
    }
    w->addServerList(lst);
    return app.exec();
}
