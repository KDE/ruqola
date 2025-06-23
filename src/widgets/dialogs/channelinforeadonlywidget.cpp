/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinforeadonlywidget.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"
#include "room.h"
#include "roomavatarreadonlywidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>

ChannelInfoReadOnlyWidget::ChannelInfoReadOnlyWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mNameReadOnly(new QLabel(this))
    , mRoomAvatarWidget(new RoomAvatarReadOnlyWidget(this))
    , mRocketChatAccount(account)
    , mLayoutReadOnly(new QFormLayout(this))
{
    mLayoutReadOnly->setObjectName(u"layoutReadOnly"_s);
    mLayoutReadOnly->setContentsMargins({});

    mRoomAvatarWidget->setObjectName(u"mRoomAvatarWidget"_s);
    mLayoutReadOnly->addRow(u" "_s, mRoomAvatarWidget);

    mNameReadOnly->setTextFormat(Qt::RichText);
    mNameReadOnly->setObjectName(u"mNameReadOnly"_s);
    mNameReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mNameReadOnly->setOpenExternalLinks(true);
    mNameReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mLayoutReadOnly->addRow(i18n("Name:"), mNameReadOnly);
}

ChannelInfoReadOnlyWidget::~ChannelInfoReadOnlyWidget() = default;

void ChannelInfoReadOnlyWidget::createDescriptionReadOnly()
{
    mDescriptionReadOnly = new QLabel(this);
    mDescriptionReadOnly->setTextFormat(Qt::RichText);
    mDescriptionReadOnly->setObjectName(u"mDescriptionReadOnly"_s);
    mDescriptionReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mDescriptionReadOnly->setOpenExternalLinks(true);
    mDescriptionReadOnly->setWordWrap(true);
    mDescriptionReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mLayoutReadOnly->addRow(i18n("Description:"), mDescriptionReadOnly);
}

void ChannelInfoReadOnlyWidget::createCommentReadOnly()
{
    mCommentReadOnly = new QLabel(this);
    mCommentReadOnly->setTextFormat(Qt::RichText);
    mCommentReadOnly->setObjectName(u"mCommentReadOnly"_s);
    mCommentReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mCommentReadOnly->setOpenExternalLinks(true);
    mCommentReadOnly->setWordWrap(true);
    mCommentReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mLayoutReadOnly->addRow(i18n("Comment:"), mCommentReadOnly);
}

void ChannelInfoReadOnlyWidget::createAnnouncementReadOnly()
{
    mAnnouncementReadOnly = new QLabel(this);
    mAnnouncementReadOnly->setTextFormat(Qt::RichText);
    mAnnouncementReadOnly->setObjectName(u"mAnnouncementReadOnly"_s);
    mAnnouncementReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mAnnouncementReadOnly->setOpenExternalLinks(true);
    mAnnouncementReadOnly->setWordWrap(true);
    mAnnouncementReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mLayoutReadOnly->addRow(i18n("Announcement:"), mAnnouncementReadOnly);
}

void ChannelInfoReadOnlyWidget::setRoom(Room *room)
{
    mRoom = room;
    updateReadOnlyChannelInfo();
    connectReadOnlyWidget();
}

void ChannelInfoReadOnlyWidget::updateReadOnlyChannelInfo()
{
    mNameReadOnly->setText(mRoom->displayFName());
    if (!mRoom->displayTopic().isEmpty()) {
        if (!mCommentReadOnly) {
            createCommentReadOnly();
        }
        mCommentReadOnly->setText(mRoom->displayTopic());
    }
    if (!mRoom->displayAnnouncement().isEmpty()) {
        if (!mAnnouncementReadOnly) {
            createAnnouncementReadOnly();
        }
        mAnnouncementReadOnly->setText(mRoom->displayAnnouncement());
    }
    if (!mRoom->description().isEmpty()) {
        if (!mDescriptionReadOnly) {
            createDescriptionReadOnly();
        }
        mDescriptionReadOnly->setText(mRoom->description());
    }
}

void ChannelInfoReadOnlyWidget::connectReadOnlyWidget()
{
    connect(mRoom, &Room::announcementChanged, this, [this]() {
        if (!mAnnouncementReadOnly) {
            createAnnouncementReadOnly();
        }
        mAnnouncementReadOnly->setText(mRoom->announcement());
    });
    connect(mRoom, &Room::topicChanged, this, [this]() {
        if (!mCommentReadOnly) {
            createCommentReadOnly();
        }
        mCommentReadOnly->setText(mRoom->topic());
    });
    connect(mRoom, &Room::nameChanged, this, [this]() {
        mNameReadOnly->setText(mRoom->name());
    });
    connect(mRoom, &Room::descriptionChanged, this, [this]() {
        if (!mDescriptionReadOnly) {
            createDescriptionReadOnly();
        }
        mDescriptionReadOnly->setText(mRoom->description());
    });
    const Utils::AvatarInfo avatarInfo = mRoom->avatarInfo();
    const QString iconUrlStr = mRocketChatAccount->avatarUrl(avatarInfo);
    mRoomAvatarWidget->setCurrentIconPath(iconUrlStr);
}

#include "moc_channelinforeadonlywidget.cpp"
