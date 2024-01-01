/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomavatarreadonlywidget.h"
#include <QIcon>
#include <QUrl>

RoomAvatarReadOnlyWidget::RoomAvatarReadOnlyWidget(QWidget *parent)
    : QLabel{parent}
{
    setFixedSize(QSize(120, 120));
}

RoomAvatarReadOnlyWidget::~RoomAvatarReadOnlyWidget() = default;

void RoomAvatarReadOnlyWidget::setCurrentIconPath(const QString &iconUrlStr)
{
    if (!iconUrlStr.isEmpty()) {
        const QIcon icon(QUrl(iconUrlStr).toLocalFile());
        setPixmap(icon.pixmap(100, 100));
    }
}

#include "moc_roomavatarreadonlywidget.cpp"
