/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "roomavatarwidget.h"
#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QMenu>

RoomAvatarWidget::RoomAvatarWidget(QWidget *parent)
    : QPushButton(parent)
{
    setIconSize(QSize(100, 100));
    setFixedSize(QSize(120, 120));

    connect(this, &RoomAvatarWidget::clicked, this, &RoomAvatarWidget::changeImage);
}

RoomAvatarWidget::~RoomAvatarWidget()
{
}

void RoomAvatarWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(i18n("Change Picture..."), this, &RoomAvatarWidget::changeImage);
    menu.addSeparator();
    menu.addAction(i18n("Reset Avatar"), this, &RoomAvatarWidget::resetAvatar);
    menu.exec(event->globalPos());
}

void RoomAvatarWidget::changeImage()
{
    // TODO
}

void RoomAvatarWidget::resetAvatar()
{
    // TODO
}
