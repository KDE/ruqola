/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grabscreenplugintoolutil.h"
using namespace Qt::Literals::StringLiterals;

#include "grabscreenplugin_debug.h"

#include <QDir>
#include <QStandardPaths>

QString GrabScreenPluginToolUtil::picturePath(const QString &accountName)
{
    const QString imagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + u"/ruqola/"_s + accountName;

    if (!QDir().mkpath(imagePath)) {
        qCWarning(RUQOLA_GRABSCREEN_PLUGIN_LOG) << "Unable to create image folder: " << imagePath;
        return {};
    }
    return imagePath;
}

QString GrabScreenPluginToolUtil::generateFileName(const QString &dirPath)
{
    const QString filename = u"screenshot"_s;
    int index = 1;
    QString newName;
    do {
        newName = dirPath + u'/' + u"%1-%2.png"_s.arg(filename, QString::number(index));
        index++;
    } while (QFile::exists(newName));
    return newName;
}
