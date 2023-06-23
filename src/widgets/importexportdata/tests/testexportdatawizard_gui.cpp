/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importexportdata/exportdata/exportdatawizard.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    // Use specific ruqola name for account name
    app.setApplicationName(QStringLiteral("ruqola"));

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();

    parser.process(app);
    auto d = new ExportDataWizard;
    d->show();

    return app.exec();
}
