/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "dialogs/showimagedialog.h"
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
    parser.addPositionalArgument({QStringLiteral("file")}, QStringLiteral("Image file"));
    QCommandLineOption isAnimatedImageOption({QStringLiteral("isAnimatedImage")},
                                             QStringLiteral("Whether the image file contains animation (e.g. for GIF files)"));
    parser.addOption(isAnimatedImageOption);
    parser.process(app);

    if (parser.positionalArguments().isEmpty()) {
        parser.showHelp(1); // exits
    }

    const QString fileName = parser.positionalArguments().value(0);

    ShowImageDialog dlg;
    ShowImageWidget::ImageInfo info;
    info.isAnimatedImage = parser.isSet(isAnimatedImageOption);
    info.imagePath = fileName;
    info.pixmap = QPixmap(fileName);
    dlg.setImageInfo(info);
    dlg.resize(800, 600);
    dlg.show();

    return app.exec();
}
