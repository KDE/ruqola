/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "dialogs/playsounddialog.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QUrl>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("+[file]"), QStringLiteral("Sound file")));

    parser.process(app);
    QString filename;
    if (!parser.positionalArguments().isEmpty()) {
        filename = parser.positionalArguments().at(0);
    }
    auto d = new PlaySoundDialog;
    d->setAudioUrl(QUrl::fromLocalFile(filename));
    d->resize(800, 600);
    d->show();

    return app.exec();
}
