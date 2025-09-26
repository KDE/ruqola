/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomutil.h"
#include "ruqolawidgets_debug.h"
#include <QScreen>

using namespace Qt::Literals::StringLiterals;
QString RoomUtil::generateUserLink(const QString &userName)
{
    return u"ruqola:/user/"_s + userName;
}

QString RoomUtil::generatePermalink(const QString &messageId, const QString &roomId, Room::RoomType channelType)
{
    QString prefix;
    switch (channelType) {
    case Room::RoomType::Channel:
        prefix = u"channel/"_s;
        break;
    case Room::RoomType::Direct:
        prefix = u"direct/"_s;
        break;
    case Room::RoomType::Private:
        prefix = u"group/"_s;
        break;
    case Room::RoomType::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << " channel type undefined " << channelType;
        break;
    }
    const QString result = u"%1%2?msg=%3"_s.arg(prefix, roomId, messageId);
    return result;
}

void RoomUtil::positionPopup(QPoint pos, QWidget *parentWindow, QWidget *popup)
{
    const QRect screenRect = parentWindow->screen()->availableGeometry();

    const QSize popupSize{popup->sizeHint()};
    QRect popupRect(QPoint(pos.x() - popupSize.width(), pos.y() - popupSize.height()), popup->sizeHint());
    if (popupRect.top() < screenRect.top()) {
        popupRect.moveTop(screenRect.top());
    }

    if ((pos.x() + popupSize.width()) > (screenRect.x() + screenRect.width())) {
        popupRect.setX(screenRect.x() + screenRect.width() - popupSize.width());
    }
    if (pos.x() - popupSize.width() < screenRect.x()) {
        popupRect.setX(screenRect.x());
    }

    popup->setGeometry(popupRect);
}
