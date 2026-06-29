/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "notification.h"

#include "ruqola_notification_debug.h"

#include <KLocalizedString>
#include <QIcon>

using namespace Qt::Literals::StringLiterals;
Notification::Notification(QObject *parent)
    : KStatusNotifierItem(parent)
{
    createTrayIcon();
}

Notification::~Notification() = default;

void Notification::createTrayIcon()
{
    setToolTipTitle(i18n("Ruqola"));

    // use icon name on Linux so that recoloring works
#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
    setIconByName(u"ruqola-symbolic"_s);
#else
    setIconByPixmap(QIcon(u":/icons/ruqola-symbolic.svg"_s));
#endif
    setCategory(KStatusNotifierItem::Communications);
}

void Notification::roomNeedAttention()
{
    qCDebug(RUQOLA_NOTIFICATION_LOG) << " emit alert";
    Q_EMIT alert();
}

void Notification::updateToolTip(const QString &str, bool hasAlert)
{
    setToolTipSubTitle(str);
    if (status() == KStatusNotifierItem::Passive && (!str.isEmpty() || hasAlert)) {
        setStatus(KStatusNotifierItem::Active);
    } else if (status() == KStatusNotifierItem::Active && (str.isEmpty() && !hasAlert)) {
        setStatus(KStatusNotifierItem::Passive);
    }
}

#include "moc_notification.cpp"
