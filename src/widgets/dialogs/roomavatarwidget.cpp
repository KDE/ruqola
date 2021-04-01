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
#include <QBuffer>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QImageReader>
#include <QMenu>

RoomAvatarWidget::RoomAvatarWidget(QWidget *parent)
    : QPushButton(parent)
{
    setIconSize(QSize(100, 100));
    setFixedSize(QSize(120, 120));

    connect(this, &RoomAvatarWidget::clicked, this, &RoomAvatarWidget::slotChangeImage);
}

RoomAvatarWidget::~RoomAvatarWidget()
{
}

void RoomAvatarWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (mReadOnly) {
        return;
    }
    QMenu menu;
    menu.addAction(i18n("Change Picture..."), this, &RoomAvatarWidget::slotChangeImage);
    menu.addSeparator();
    menu.addAction(i18n("Reset Avatar"), this, &RoomAvatarWidget::slotResetAvatar);
    menu.addSeparator();
    menu.addAction(QIcon::fromTheme(QStringLiteral("dialog-cancel")), i18n("Cancel"), this, &RoomAvatarWidget::slotCancel);
    menu.exec(event->globalPos());
}

void RoomAvatarWidget::slotChangeImage()
{
    if (mReadOnly) {
        return;
    }
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

void RoomAvatarWidget::slotCancel()
{
    mWasChanged = false;
    setIcon(QIcon(mCurrentIconPath));
}

void RoomAvatarWidget::slotResetAvatar()
{
    mRoomAvatarPath.clear();
    setIcon(QIcon());
    // Fetch default icons.
    mWasChanged = true;
}

bool RoomAvatarWidget::readOnly() const
{
    return mReadOnly;
}

void RoomAvatarWidget::setCurrentIconPath(const QString &currentPath)
{
    mCurrentIconPath = currentPath;
    setIcon(QIcon(mCurrentIconPath));
}

bool RoomAvatarWidget::wasChanged() const
{
    return mWasChanged;
}

void RoomAvatarWidget::setReadOnly(bool state)
{
    mReadOnly = state;
}

QString RoomAvatarWidget::roomAvatar() const
{
    if (mRoomAvatarPath.isEmpty()) {
        return {};
    } else {
        QImage b(mRoomAvatarPath);
        QByteArray ba;
        QBuffer buf(&ba);
        b.save(&buf, "png");
        const QByteArray hexed = ba.toBase64();
        buf.close();
        return QString::fromUtf8(QByteArray("data:image/png;base64,") + hexed);
    }
}
