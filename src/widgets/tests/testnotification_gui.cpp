/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationwidget.h"
#include <QApplication>
#include <QCommandLineParser>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    // Use specific ruqola name for database path
    app.setApplicationName(QStringLiteral("ruqola"));
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);
    auto w = new NotificationWidget();
    w->resize(800, 600);
    w->show();
    return app.exec();
}
