/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grabscreenplugintoolutil.h"

#include "grabscreenplugin_debug.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

using namespace Qt::Literals::StringLiterals;
QString GrabScreenPluginToolUtil::picturePath(const QString &accountName)
{
    QString imagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + u"/ruqola/"_s + accountName;

    if (!QDir().mkpath(imagePath)) {
        qCWarning(RUQOLA_GRABSCREEN_PLUGIN_LOG) << "Unable to create image folder: " << imagePath;
        return {};
    }
    return imagePath;
}

QString GrabScreenPluginToolUtil::generateFileName(const QString &dirPath)
{
    int index = 1;
    QString newName;
    do {
        newName = dirPath + u"/screenshot-%1.png"_s.arg(index);
        index++;
    } while (QFile::exists(newName));
    return newName;
}
