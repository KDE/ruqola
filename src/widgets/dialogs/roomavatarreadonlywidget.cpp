/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomavatarreadonlywidget.h"
#include <QIcon>

RoomAvatarReadOnlyWidget::RoomAvatarReadOnlyWidget(QWidget *parent)
    : QLabel{parent}
{
    setFixedSize(QSize(120, 120));
}

RoomAvatarReadOnlyWidget::~RoomAvatarReadOnlyWidget() = default;

void RoomAvatarReadOnlyWidget::setCurrentIconPath(const QString &currentPath)
{
    // TODO verify it
    setPixmap(QPixmap(currentPath));
}
