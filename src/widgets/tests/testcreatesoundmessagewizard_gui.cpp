/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "dialogs/createsoundmessagewizard.h"
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
    auto d = new CreateSoundMessageWizard(nullptr);
    d->show();

    return app.exec();
}
