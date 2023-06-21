/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinforeadonlywidget.h"
#include "rocketchataccount.h"
#include "room.h"
#include "roomavatarreadonlywidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>

ChannelInfoReadOnlyWidget::ChannelInfoReadOnlyWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mNameReadOnly(new QLabel(this))
    , mCommentReadOnly(new QLabel(this))
    , mAnnouncementReadOnly(new QLabel(this))
    , mDescriptionReadOnly(new QLabel(this))
    , mRoomAvatarWidget(new RoomAvatarReadOnlyWidget(this))
    , mRocketChatAccount(account)
{
    auto layoutReadOnly = new QFormLayout(this);
    layoutReadOnly->setObjectName(QStringLiteral("layoutReadOnly"));
    layoutReadOnly->setContentsMargins({});

    mRoomAvatarWidget->setObjectName(QStringLiteral("mRoomAvatarWidget"));
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
    mRoomAvatarWidget->setCurrentIconPath(iconUrlStr);
}

#include "moc_channelinforeadonlywidget.cpp"
