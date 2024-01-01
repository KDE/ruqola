/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectedmessagebackgroundanimationwidget.h"

#include <QApplication>
#include <QCommandLineParser>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    auto w = new SelectedMessageBackgroundAnimationWidget();
    w->resize(800, 600);
    w->show();

    return app.exec();
}
