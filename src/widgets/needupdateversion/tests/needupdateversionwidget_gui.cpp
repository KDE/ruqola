/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include <QApplication>
#include <QCommandLineParser>
#include <QStandardPaths>

#include "needupdateversionmainwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    QStandardPaths::setTestModeEnabled(true);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    auto needUpdateWidget = new NeedUpdateVersionMainWidget;
    needUpdateWidget->show();

    return app.exec();
}
