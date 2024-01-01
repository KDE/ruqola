/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

RoomAvatarWidget::~RoomAvatarWidget() = default;

void RoomAvatarWidget::contextMenuEvent(QContextMenuEvent *event)
{
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

void RoomAvatarWidget::setCurrentIconPath(const QString &currentPath)
{
    mCurrentIconPath = currentPath;
    setIcon(QIcon(mCurrentIconPath));
}

bool RoomAvatarWidget::wasChanged() const
{
    return mWasChanged;
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

#include "moc_roomavatarwidget.cpp"
