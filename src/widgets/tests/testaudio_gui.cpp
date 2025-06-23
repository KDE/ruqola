/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "dialogs/playsounddialog.h"
using namespace Qt::Literals::StringLiterals;

#include <QApplication>
#include <QCommandLineParser>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << u"+[file]"_s, u"Sound file"_s));

    parser.process(app);
    QString filename;
    if (!parser.positionalArguments().isEmpty()) {
        filename = parser.positionalArguments().at(0);
    }
    auto d = new PlaySoundDialog(nullptr);
    d->setAudioPath(filename);
    d->resize(800, 600);
    d->show();

    return app.exec();
}
