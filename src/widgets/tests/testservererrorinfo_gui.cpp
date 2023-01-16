/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfohistory/servererrorinfomessagehistorydialog.h"
#include "servererrorinfohistorymanager.h"
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
    ServerErrorInfo info;
    info.setAccountName(QStringLiteral("account 1"));
    info.setMessage(QStringLiteral("blabla"));
    info.setDateTime(QDateTime::currentDateTime());
    ServerErrorInfoHistoryManager::self()->addServerErrorInfo(info);

    return app.exec();
}
