/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "dialogs/showimagedialog.h"
using namespace Qt::Literals::StringLiterals;

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QPixmap>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addPositionalArgument({u"file"_s}, u"Image file"_s);
    const QCommandLineOption isAnimatedImageOption({u"isAnimatedImage"_s}, u"Whether the image file contains animation (e.g. for GIF files)"_s);
    parser.addOption(isAnimatedImageOption);
    parser.process(app);

    if (parser.positionalArguments().isEmpty()) {
        parser.showHelp(1); // exits
    }

    const QString fileName = parser.positionalArguments().value(0);

    ShowImageDialog dlg(nullptr);
    ShowImageWidget::ImageInfo info;
    info.isAnimatedImage = parser.isSet(isAnimatedImageOption);
    info.bigImagePath = fileName;
    info.pixmap = QPixmap(fileName);
    dlg.setImageInfo(info);
    dlg.resize(800, 600);
    dlg.show();

    return app.exec();
}
