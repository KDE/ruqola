/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "dialogs/createvideomessagewizard.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QStandardPaths>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();

    parser.process(app);
    auto d = new CreateVideoMessageWizard(nullptr);
    d->resize(800, 600);
    d->show();

    return app.exec();
}
