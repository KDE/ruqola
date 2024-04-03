/*

 * SPDX-FileCopyrightText: 2017 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ruqolautils.h"
#include "config-ruqola.h"
#include "ruqola_debug.h"
#include "utils.h"
#include <QDesktopServices>
#include <QUrl>
#ifdef HAS_ACTIVITY_SUPPORT
#include <PlasmaActivities/ResourceInstance>
#endif

RuqolaUtils::RuqolaUtils(QObject *parent)
    : QObject(parent)
{
}

RuqolaUtils::~RuqolaUtils() = default;

RuqolaUtils *RuqolaUtils::self()
{
    static RuqolaUtils s_self;
    return &s_self;
}

QString RuqolaUtils::extractRoomUserFromUrl(const QString &url)
{
    return Utils::extractRoomUserFromUrl(url);
}

void RuqolaUtils::openUrl(const QString &url)
{
    const QUrl clickedUrl = QUrl::fromUserInput(url);
    openUrl(clickedUrl);
}

void RuqolaUtils::openUrl(const QUrl &url)
{
    if (QDesktopServices::openUrl(url)) {
#if HAS_ACTIVITY_SUPPORT
        KActivities::ResourceInstance::notifyAccessed(url);
#endif
    } else {
        qCWarning(RUQOLA_LOG) << "Impossible to open " << url;
    }
}

#include "moc_ruqolautils.cpp"
