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

#include "channelinforeadonlywidget.h"
#include "rocketchataccount.h"
#include "room.h"
#include "roomavatarwidget.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>

ChannelInfoReadOnlyWidget::ChannelInfoReadOnlyWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mNameReadOnly(new QLabel(this))
    , mCommentReadOnly(new QLabel(this))
    , mAnnouncementReadOnly(new QLabel(this))
    , mDescriptionReadOnly(new QLabel(this))
    , mRoomAvatarWidget(new RoomAvatarWidget(this))
    , mRocketChatAccount(account)
{
    auto layoutReadOnly = new QFormLayout(this);
    layoutReadOnly->setObjectName(QStringLiteral("layoutReadOnly"));
    layoutReadOnly->setContentsMargins({});

    mRoomAvatarWidget->setObjectName(QStringLiteral("mRoomAvatarWidget"));
    mRoomAvatarWidget->setReadOnly(true);
    layoutReadOnly->addRow(QStringLiteral(" "), mRoomAvatarWidget);

    mNameReadOnly->setTextFormat(Qt::RichText);
    mNameReadOnly->setObjectName(QStringLiteral("mNameReadOnly"));
    mNameReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mNameReadOnly->setOpenExternalLinks(true);
    mNameReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layoutReadOnly->addRow(i18n("Name:"), mNameReadOnly);

    mCommentReadOnly->setTextFormat(Qt::RichText);
    mCommentReadOnly->setObjectName(QStringLiteral("mCommentReadOnly"));
    mCommentReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mCommentReadOnly->setOpenExternalLinks(true);
    mCommentReadOnly->setWordWrap(true);
    mCommentReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layoutReadOnly->addRow(i18n("Comment:"), mCommentReadOnly);

    mAnnouncementReadOnly->setTextFormat(Qt::RichText);
    mAnnouncementReadOnly->setObjectName(QStringLiteral("mAnnouncementReadOnly"));
    mAnnouncementReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mAnnouncementReadOnly->setOpenExternalLinks(true);
    mAnnouncementReadOnly->setWordWrap(true);
    mAnnouncementReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layoutReadOnly->addRow(i18n("Announcement:"), mAnnouncementReadOnly);

    mDescriptionReadOnly->setTextFormat(Qt::RichText);
    mDescriptionReadOnly->setObjectName(QStringLiteral("mDescriptionReadOnly"));
    mDescriptionReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mDescriptionReadOnly->setOpenExternalLinks(true);
    mDescriptionReadOnly->setWordWrap(true);
    mDescriptionReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layoutReadOnly->addRow(i18n("Description:"), mDescriptionReadOnly);
}

ChannelInfoReadOnlyWidget::~ChannelInfoReadOnlyWidget() = default;

void ChannelInfoReadOnlyWidget::setRoom(Room *room)
{
    mRoom = room;
    updateReadOnlyChannelInfo();
    connectReadOnlyWidget();
}

void ChannelInfoReadOnlyWidget::updateReadOnlyChannelInfo()
{
    mNameReadOnly->setText(mRoom->displayFName());
    mCommentReadOnly->setText(mRoom->displayTopic());
    mAnnouncementReadOnly->setText(mRoom->displayAnnouncement());
    mDescriptionReadOnly->setText(mRoom->description());
}

void ChannelInfoReadOnlyWidget::connectReadOnlyWidget()
{
    connect(mRoom, &Room::announcementChanged, this, [this]() {
        mAnnouncementReadOnly->setText(mRoom->announcement());
    });
    connect(mRoom, &Room::topicChanged, this, [this]() {
        mCommentReadOnly->setText(mRoom->topic());
    });
    connect(mRoom, &Room::nameChanged, this, [this]() {
        mNameReadOnly->setText(mRoom->name());
    });
    connect(mRoom, &Room::descriptionChanged, this, [this]() {
        mDescriptionReadOnly->setText(mRoom->description());
    });
    const Utils::AvatarInfo avatarInfo = mRoom->avatarInfo();
    const QString iconUrlStr = mRocketChatAccount->avatarUrl(avatarInfo);
    if (!iconUrlStr.isEmpty()) {
        const QIcon icon(QUrl(iconUrlStr).toLocalFile());
        mRoomAvatarWidget->setIcon(icon);
    }
}
