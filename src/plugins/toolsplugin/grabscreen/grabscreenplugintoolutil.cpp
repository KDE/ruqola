/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grabscreenplugintoolutil.h"
#include "grabscreenplugin_debug.h"

#include <QDir>
#include <QStandardPaths>

QString GrabScreenPluginToolUtil::picturePath()
{
    const QString imagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + QStringLiteral("/ruqola/");

    if (!QDir().mkpath(imagePath)) {
        qCWarning(RUQOLA_GRABSCREEN_PLUGIN_LOG) << "Unable to create image folder: " << imagePath;
        return {};
    }
    return imagePath;
}
