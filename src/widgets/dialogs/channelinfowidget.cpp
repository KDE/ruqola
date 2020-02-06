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

#include "channelinfowidget.h"
#include "roomwrapper.h"
#include "ruqola.h"
#include "rocketchataccount.h"

#include <KLineEdit>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <KMessageBox>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QToolButton>
#include <QInputDialog>

ChannelInfoWidget::ChannelInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    //Editable channel
    mEditableChannel = new QWidget(this);
    mEditableChannel->setObjectName(QStringLiteral("mEditableChannel"));
    mStackedWidget->addWidget(mEditableChannel);

    auto *layout = new QFormLayout(mEditableChannel);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins(0, 0, 0, 0);

    mName = new ChangeTextWidget(this);
    mName->setObjectName(QStringLiteral("mName"));
    connect(mName, &ChangeTextWidget::textChanged, this, [this](const QString &name) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::Name, name, mRoomWrapper->channelType());
    });
    layout->addRow(i18n("Name:"), mName);

    mComment = new ChangeTextWidget(this);
    mComment->setObjectName(QStringLiteral("mComment"));
    layout->addRow(i18n("Comment:"), mComment);
    connect(mComment, &ChangeTextWidget::textChanged, this, [this](const QString &name) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::Topic, name, mRoomWrapper->channelType());
    });

    mAnnouncement = new ChangeTextWidget(this);
    mAnnouncement->setObjectName(QStringLiteral("mAnnouncement"));
    connect(mAnnouncement, &ChangeTextWidget::textChanged, this, [this](const QString &name) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::Announcement, name, mRoomWrapper->channelType());
    });
    layout->addRow(i18n("Announcement:"), mAnnouncement);

    mDescription = new ChangeTextWidget(this);
    mDescription->setObjectName(QStringLiteral("mDescription"));
    connect(mDescription, &ChangeTextWidget::textChanged, this, [this](const QString &name) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::Description, name, mRoomWrapper->channelType());
    });

    layout->addRow(i18n("Description:"), mDescription);

    mPasswordLineEdit = new KPasswordLineEdit(this);
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    layout->addRow(i18n("Password:"), mPasswordLineEdit);
    //TODO

    mReadOnly = new QCheckBox(this);
    mReadOnly->setObjectName(QStringLiteral("mReadOnly"));
    layout->addRow(i18n("ReadOnly:"), mReadOnly);
    connect(mReadOnly, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::ReadOnly, checked, mRoomWrapper->channelType());
    });

    mArchive = new QCheckBox(this);
    mArchive->setObjectName(QStringLiteral("mArchive"));
    layout->addRow(i18n("Archive:"), mArchive);
    connect(mArchive, &QCheckBox::clicked, this, [this](bool checked) {
        const QString text = checked ? i18n("Do you want to archive this room?") : i18n("Do you want to unarchive this room?");
        const QString title = checked ? i18n("Archive Channel") : i18n("Unarchive Channel");
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, text, title)) {
            Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::Archive, checked, mRoomWrapper->channelType());
        }
    });

    mPrivate = new QCheckBox(this);
    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    layout->addRow(i18n("Private:"), mPrivate);
    connect(mPrivate, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::RoomType, checked, mRoomWrapper->channelType());
    });

    //TODO add encrypted too!

    mDeleteChannel = new QPushButton(i18n("Delete"), this); //TODO add icons!
    mDeleteChannel->setObjectName(QStringLiteral("mDeleteChannel"));
    layout->addRow(QStringLiteral(" "), mDeleteChannel);
    connect(mDeleteChannel, &QPushButton::clicked, this, [this]() {
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to delete this room?"), i18n("Delete Room"))) {
            Ruqola::self()->rocketChatAccount()->eraseRoom(mRoomWrapper->roomId(), mRoomWrapper->channelType());
            Q_EMIT channelDeleted();
        }
    });

    //ReadOnly Channel
    mReadOnlyChannel = new QWidget(this);
    mReadOnlyChannel->setObjectName(QStringLiteral("mReadOnlyChannel"));
    mStackedWidget->addWidget(mReadOnlyChannel);

    auto *layoutReadOnly = new QFormLayout(mReadOnlyChannel);
    layoutReadOnly->setObjectName(QStringLiteral("layoutReadOnly"));
    layoutReadOnly->setContentsMargins(0, 0, 0, 0);

    mNameReadOnly = new QLabel(this);
    mNameReadOnly->setTextFormat(Qt::RichText);
    mNameReadOnly->setObjectName(QStringLiteral("mNameReadOnly"));
    mNameReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mNameReadOnly->setOpenExternalLinks(true);
    layoutReadOnly->addRow(i18n("Name:"), mNameReadOnly);

    mCommentReadOnly = new QLabel(this);
    mCommentReadOnly->setTextFormat(Qt::RichText);
    mCommentReadOnly->setObjectName(QStringLiteral("mCommentReadOnly"));
    mCommentReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mCommentReadOnly->setOpenExternalLinks(true);
    layoutReadOnly->addRow(i18n("Comment:"), mCommentReadOnly);

    mAnnouncementReadOnly = new QLabel(this);
    mAnnouncementReadOnly->setTextFormat(Qt::RichText);
    mAnnouncementReadOnly->setObjectName(QStringLiteral("mAnnouncementReadOnly"));
    mAnnouncementReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mAnnouncementReadOnly->setOpenExternalLinks(true);
    layoutReadOnly->addRow(i18n("Announcement:"), mAnnouncementReadOnly);

    mDescriptionReadOnly = new QLabel(this);
    mDescriptionReadOnly->setTextFormat(Qt::RichText);
    mDescriptionReadOnly->setObjectName(QStringLiteral("mDescriptionReadOnly"));
    mDescriptionReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mDescriptionReadOnly->setOpenExternalLinks(true);
    layoutReadOnly->addRow(i18n("Description:"), mDescriptionReadOnly);
}

ChannelInfoWidget::~ChannelInfoWidget()
{
}

void ChannelInfoWidget::setRoomWrapper(RoomWrapper *roomWrapper)
{
    mRoomWrapper = roomWrapper;
    if (mRoomWrapper->canBeModify()) {
        mStackedWidget->setCurrentWidget(mEditableChannel);
        updateEditableChannelInfo();
        connectReadOnlyWidget();
    } else {
        mStackedWidget->setCurrentWidget(mReadOnlyChannel);
        updateReadOnlyChannelInfo();
        connectReadOnlyWidget();
    }
}

void ChannelInfoWidget::updateReadOnlyChannelInfo()
{
    mNameReadOnly->setText(mRoomWrapper->name());
    mCommentReadOnly->setText(mRoomWrapper->topic());
    mAnnouncementReadOnly->setText(mRoomWrapper->announcement());
    mDescriptionReadOnly->setText(mRoomWrapper->description());
}

void ChannelInfoWidget::updateEditableChannelInfo()
{
    mName->setText(mRoomWrapper->name());
    mComment->setText(mRoomWrapper->topic());
    mAnnouncement->setText(mRoomWrapper->announcement());
    mDescription->setText(mRoomWrapper->description());
    mReadOnly->setChecked(mRoomWrapper->readOnly());
    mArchive->setChecked(mRoomWrapper->archived());
    mPrivate->setChecked(mRoomWrapper->channelType() == QStringLiteral("p"));
}

void ChannelInfoWidget::connectReadOnlyWidget()
{
    connect(mRoomWrapper, &RoomWrapper::announcementChanged, this, [this]() {
        mAnnouncementReadOnly->setText(mRoomWrapper->announcement());
    });
    connect(mRoomWrapper, &RoomWrapper::topicChanged, this, [this]() {
        mCommentReadOnly->setText(mRoomWrapper->topic());
    });
    connect(mRoomWrapper, &RoomWrapper::nameChanged, this, [this]() {
        mNameReadOnly->setText(mRoomWrapper->name());
    });
    connect(mRoomWrapper, &RoomWrapper::descriptionChanged, this, [this]() {
        mDescriptionReadOnly->setText(mRoomWrapper->description());
    });
}

void ChannelInfoWidget::connectEditableWidget()
{
    connect(mRoomWrapper, &RoomWrapper::announcementChanged, this, [this]() {
        mAnnouncement->setText(mRoomWrapper->announcement());
    });
    connect(mRoomWrapper, &RoomWrapper::topicChanged, this, [this]() {
        mComment->setText(mRoomWrapper->topic());
    });
    connect(mRoomWrapper, &RoomWrapper::nameChanged, this, [this]() {
        mName->setText(mRoomWrapper->name());
    });
    connect(mRoomWrapper, &RoomWrapper::descriptionChanged, this, [this]() {
        mDescription->setText(mRoomWrapper->description());
    });
    connect(mRoomWrapper, &RoomWrapper::readOnlyChanged, this, [this]() {
        mReadOnly->setChecked(mRoomWrapper->readOnly());
    });
    connect(mRoomWrapper, &RoomWrapper::archivedChanged, this, [this]() {
        mArchive->setChecked(mRoomWrapper->archived());
    });
    connect(mRoomWrapper, &RoomWrapper::channelTypeChanged, this, [this]() {
        mPrivate->setChecked(mRoomWrapper->channelType() == QStringLiteral("p"));
    });
    //TODO react when we change settings
    connect(mReadOnly, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::ReadOnly, checked, mRoomWrapper->channelType());
    });
    connect(mArchive, &QCheckBox::clicked, this, [this](bool checked) {
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, checked ? i18n("Do you want to archive this room?") : i18n("Do you want to unarchive this room?"), i18n("Archive room"))) {
            Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::Archive, checked, mRoomWrapper->channelType());
        }
    });
    connect(mPrivate, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoomWrapper->roomId(), RocketChatAccount::RoomType, checked, mRoomWrapper->channelType());
    });
}

ChangeTextWidget::ChangeTextWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mLabel = new QLabel(this);
    mLabel->setObjectName(QStringLiteral("mLabel"));
    mLabel->setWordWrap(true);
    mLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mLabel->setOpenExternalLinks(true);

    mainLayout->addWidget(mLabel);
    mainLayout->addStretch(1);
    mChangeTextToolButton = new QToolButton(this);
    mChangeTextToolButton->setIcon(QIcon::fromTheme(QStringLiteral("document-edit")));
    mChangeTextToolButton->setObjectName(QStringLiteral("mChangeTextToolButton"));
    mainLayout->addWidget(mChangeTextToolButton);
    connect(mChangeTextToolButton, &QToolButton::clicked, this, [this]() {
        const QString result = QInputDialog::getText(this, i18n("Change Text"), i18n("Text:"), QLineEdit::Normal, mLabel->text());
        if (!result.trimmed().isEmpty()) {
            if (result != mLabel->text()) {
                Q_EMIT textChanged(result);
            }
        }
    });
}

ChangeTextWidget::~ChangeTextWidget()
{
}

void ChangeTextWidget::setText(const QString &str)
{
    mLabel->setText(str);
}
