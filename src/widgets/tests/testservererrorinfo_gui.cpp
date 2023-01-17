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

    for (int i = 0; i < 10; ++i) {
        ServerErrorInfo info;
        info.setAccountName(QStringLiteral("account %1").arg(QString::number(i)));
        const QString str = QStringLiteral("Message: %1").arg(QString::number(i));
        info.setMessage(str);
        ServerErrorInfoHistoryManager::self()->addServerErrorInfo(info);
    }

    return app.exec();
}
