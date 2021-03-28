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
#include <QFileDialog>
#include <QImageReader>
#include <QMenu>

RoomAvatarWidget::RoomAvatarWidget(QWidget *parent)
    : QPushButton(parent)
{
    setIconSize(QSize(100, 100));
    setFixedSize(QSize(120, 120));

    connect(this, &RoomAvatarWidget::clicked, this, &RoomAvatarWidget::changeImage);
    // TODO fetch roomAvatar
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
    QString filter;
    const QList<QByteArray> supportedImage = QImageReader::supportedImageFormats();
    for (const QByteArray &ba : supportedImage) {
        if (!filter.isEmpty()) {
            filter += QLatin1Char(' ');
        }
        filter += QLatin1String("*.") + QString::fromLatin1(ba);
    }
    filter = QStringLiteral("%1 (%2)").arg(i18n("Image"), filter);
    const QString fileName = QFileDialog::getOpenFileName(this, i18n("Select Room Avatar"), QString(), filter);
    if (!fileName.isEmpty()) {
        mRoomAvatarPath = fileName;
        const QIcon img(mRoomAvatarPath);
        setIcon(img);
        mWasChanged = true;
    }
}

void RoomAvatarWidget::resetAvatar()
{
    mRoomAvatarPath.clear();
    setIcon(QIcon());
    // Fetch default icons.
    mWasChanged = true;
}

bool RoomAvatarWidget::wasChanged() const
{
    return mWasChanged;
}

QString RoomAvatarWidget::roomAvatarPath() const
{
    return mRoomAvatarPath;
}
