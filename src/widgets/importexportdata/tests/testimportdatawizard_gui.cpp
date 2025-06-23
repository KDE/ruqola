/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importexportdata/importdata/importdatawizard.h"
using namespace Qt::Literals::StringLiterals;

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    // Use specific ruqola name for account name
    app.setApplicationName(u"ruqola"_s);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();

    parser.process(app);
    auto d = new ImportDataWizard;
    d->show();

    return app.exec();
}
