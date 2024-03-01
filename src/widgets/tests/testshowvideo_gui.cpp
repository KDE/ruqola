/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "dialogs/showvideodialog.h"
#include <QApplication>
#include <QCommandLineParser>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("+[file]"), QStringLiteral("Video file")));

    parser.process(app);
    QString filename;
    if (!parser.positionalArguments().isEmpty()) {
        filename = parser.positionalArguments().at(0);
    }
    auto d = new ShowVideoDialog(nullptr);
    d->setVideoPath(filename);
    d->resize(800, 600);
    d->show();

    return app.exec();
}
