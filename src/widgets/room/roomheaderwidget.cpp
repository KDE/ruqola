/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "roomheaderwidget.h"
#include "ruqolawidgets_debug.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QLabel>
#include <QToolButton>

RoomHeaderWidget::RoomHeaderWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto *headerLayout = new QHBoxLayout;
    headerLayout->setObjectName(QStringLiteral("headerLayout"));
    headerLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(headerLayout);

    mFavoriteButton = new QToolButton(this);
    mFavoriteButton->setObjectName(QStringLiteral("mFavoriteButton"));
    mFavoriteButton->setIcon(QIcon::fromTheme(QStringLiteral("favorite")));
    mFavoriteButton->setCheckable(true);
    headerLayout->addWidget(mFavoriteButton, Qt::AlignTop);
    connect(mFavoriteButton, &QToolButton::clicked, this, &RoomHeaderWidget::favoriteChanged);

    mEncryptedButton = new QToolButton(this);
    mEncryptedButton->setObjectName(QStringLiteral("mEncryptedButton"));
    mEncryptedButton->setIcon(QIcon::fromTheme(QStringLiteral("encrypted")));
    mEncryptedButton->setCheckable(true);
    mEncryptedButton->setVisible(false);
    headerLayout->addWidget(mEncryptedButton, Qt::AlignTop);
    connect(mEncryptedButton, &QToolButton::clicked, this, &RoomHeaderWidget::encryptedChanged);

    auto *infoLayout = new QVBoxLayout;
    infoLayout->setObjectName(QStringLiteral("infoLayout"));
    infoLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->addLayout(infoLayout);

    mRoomName = new QLabel(this);
    mRoomName->setObjectName(QStringLiteral("mRoomName"));
    mRoomName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    infoLayout->addWidget(mRoomName);
    mRoomName->setVisible(false);

    mTopic = new QLabel(this);
    mTopic->setObjectName(QStringLiteral("mTopic"));
    infoLayout->addWidget(mTopic);
    mTopic->setWordWrap(true);
    mTopic->setOpenExternalLinks(true);
    mTopic->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mTopic->setTextFormat(Qt::RichText);
    mTopic->setVisible(false);

    mAnnouncement = new QLabel(this);
    mAnnouncement->setObjectName(QStringLiteral("mAnnouncement"));
    mAnnouncement->setWordWrap(true);
    mAnnouncement->setTextFormat(Qt::RichText);
    infoLayout->addWidget(mAnnouncement);
    mAnnouncement->setOpenExternalLinks(true);
    mAnnouncement->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mAnnouncement->setVisible(false);

    mDescription = new QLabel(this);
    mDescription->setObjectName(QStringLiteral("mDescription"));
    mDescription->setTextFormat(Qt::RichText);
    mDescription->setWordWrap(true);
    infoLayout->addWidget(mDescription);
    mDescription->setOpenExternalLinks(true);
    mDescription->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mDescription->setVisible(false);
}

RoomHeaderWidget::~RoomHeaderWidget()
{
}

void RoomHeaderWidget::setRoomName(const QString &name)
{
    mRoomName->setText(name);
    mRoomName->setVisible(!name.isEmpty());
}

QString RoomHeaderWidget::roomName() const
{
    return mRoomName->text();
}

void RoomHeaderWidget::setRoomAnnouncement(const QString &name)
{
    mAnnouncement->setText(name);
    mAnnouncement->setVisible(!name.isEmpty());
}

void RoomHeaderWidget::setRoomTopic(const QString &name)
{
    mTopic->setText(name);
    mTopic->setVisible(!name.isEmpty());
}

void RoomHeaderWidget::setFavoriteStatus(bool b)
{
    mFavoriteButton->setChecked(b);
}

void RoomHeaderWidget::setEncypted(bool b)
{
    mEncryptedButton->setChecked(b);
    mEncryptedButton->setVisible(b);
}
