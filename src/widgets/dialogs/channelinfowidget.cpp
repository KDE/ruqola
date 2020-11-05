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
#include "room.h"
#include "ruqola.h"
#include "rocketchataccount.h"

#include <QLineEdit>
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
#include <QTextDocument>
#include <QHBoxLayout>

ChannelInfoWidget::ChannelInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    //Editable channel
    mEditableChannel = new QWidget(this);
    mEditableChannel->setObjectName(QStringLiteral("mEditableChannel"));
    mStackedWidget->addWidget(mEditableChannel);

    auto *layout = new QFormLayout(mEditableChannel);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

    QString str = i18n("Name:");
    mName = new ChangeTextWidget(this);
    mName->setObjectName(QStringLiteral("mName"));
    connect(mName, &ChangeTextWidget::textChanged, this, [this](const QString &name) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Name, name, mRoom->channelType());
    });
    mName->setLabelText(str);
    layout->addRow(str, mName);

    mComment = new ChangeTextWidget(this);
    mComment->setObjectName(QStringLiteral("mComment"));
    str = i18n("Comment:");
    mComment->setLabelText(str);
    layout->addRow(str, mComment);
    connect(mComment, &ChangeTextWidget::textChanged, this, [this](const QString &name) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Topic, name, mRoom->channelType());
    });

    mAnnouncement = new ChangeTextWidget(this);
    mAnnouncement->setObjectName(QStringLiteral("mAnnouncement"));
    connect(mAnnouncement, &ChangeTextWidget::textChanged, this, [this](const QString &name) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Announcement, name, mRoom->channelType());
    });
    str = i18n("Announcement:");
    mAnnouncement->setLabelText(str);
    layout->addRow(str, mAnnouncement);

    mDescription = new ChangeTextWidget(this);
    mDescription->setObjectName(QStringLiteral("mDescription"));
    connect(mDescription, &ChangeTextWidget::textChanged, this, [this](const QString &name) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Description, name, mRoom->channelType());
    });
    str = i18n("Description:");
    mDescription->setLabelText(str);

    layout->addRow(str, mDescription);

    //Show it if room is not private
    mPasswordLineEdit = new KPasswordLineEdit(this);
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    layout->addRow(i18n("Password:"), mPasswordLineEdit);
    connect(mPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, [](const QString &password) {
        qWarning() << "change password not implemented yet ";
        //Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Password, password, mRoom->channelType());
    });

    mReadOnly = new QCheckBox(this);
    mReadOnly->setObjectName(QStringLiteral("mReadOnly"));
    layout->addRow(i18n("ReadOnly:"), mReadOnly);
    connect(mReadOnly, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::ReadOnly, checked, mRoom->channelType());
    });

    mArchive = new QCheckBox(this);
    mArchive->setObjectName(QStringLiteral("mArchive"));
    layout->addRow(i18n("Archive:"), mArchive);
    connect(mArchive, &QCheckBox::clicked, this, [this](bool checked) {
        const QString text = checked ? i18n("Do you want to archive this room?") : i18n("Do you want to unarchive this room?");
        const QString title = checked ? i18n("Archive Channel") : i18n("Unarchive Channel");
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, text, title)) {
            Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Archive, checked, mRoom->channelType());
        }
    });

    mPrivate = new QCheckBox(this);
    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    layout->addRow(i18n("Private:"), mPrivate);
    connect(mPrivate, &QCheckBox::clicked, this, [this, layout](bool checked) {
        mEncrypted->setVisible(checked);
        layout->labelForField(mEncrypted)->setVisible(checked);
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::RoomType, checked, mRoom->channelType());
    });

    mEncrypted = new QCheckBox(this);
    mEncrypted->setObjectName(QStringLiteral("mEncrypted"));
    layout->addRow(i18n("Encrypted:"), mEncrypted);
    connect(mEncrypted, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Encrypted, checked, mRoom->channelType());
    });
    mEncryptedLabel = layout->labelForField(mEncrypted);

    mDeleteChannel = new QPushButton(QIcon::fromTheme(QStringLiteral("edit-delete-shred")), i18n("Delete"), this);
    mDeleteChannel->setObjectName(QStringLiteral("mDeleteChannel"));
    layout->addRow(QStringLiteral(" "), mDeleteChannel);
    connect(mDeleteChannel, &QPushButton::clicked, this, [this]() {
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to delete this room?"), i18n("Delete Room"))) {
            Ruqola::self()->rocketChatAccount()->eraseRoom(mRoom->roomId(), mRoom->channelType());
            Q_EMIT channelDeleted();
        }
    });

    //ReadOnly Channel
    mReadOnlyChannel = new QWidget(this);
    mReadOnlyChannel->setObjectName(QStringLiteral("mReadOnlyChannel"));
    mStackedWidget->addWidget(mReadOnlyChannel);

    auto *layoutReadOnly = new QFormLayout(mReadOnlyChannel);
    layoutReadOnly->setObjectName(QStringLiteral("layoutReadOnly"));
    layoutReadOnly->setContentsMargins({});

    mNameReadOnly = new QLabel(this);
    mNameReadOnly->setTextFormat(Qt::RichText);
    mNameReadOnly->setObjectName(QStringLiteral("mNameReadOnly"));
    mNameReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mNameReadOnly->setOpenExternalLinks(true);
    mNameReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layoutReadOnly->addRow(i18n("Name:"), mNameReadOnly);

    mCommentReadOnly = new QLabel(this);
    mCommentReadOnly->setTextFormat(Qt::RichText);
    mCommentReadOnly->setObjectName(QStringLiteral("mCommentReadOnly"));
    mCommentReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mCommentReadOnly->setOpenExternalLinks(true);
    mCommentReadOnly->setWordWrap(true);
    mCommentReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layoutReadOnly->addRow(i18n("Comment:"), mCommentReadOnly);

    mAnnouncementReadOnly = new QLabel(this);
    mAnnouncementReadOnly->setTextFormat(Qt::RichText);
    mAnnouncementReadOnly->setObjectName(QStringLiteral("mAnnouncementReadOnly"));
    mAnnouncementReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mAnnouncementReadOnly->setOpenExternalLinks(true);
    mAnnouncementReadOnly->setWordWrap(true);
    mAnnouncementReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layoutReadOnly->addRow(i18n("Announcement:"), mAnnouncementReadOnly);

    mDescriptionReadOnly = new QLabel(this);
    mDescriptionReadOnly->setTextFormat(Qt::RichText);
    mDescriptionReadOnly->setObjectName(QStringLiteral("mDescriptionReadOnly"));
    mDescriptionReadOnly->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mDescriptionReadOnly->setOpenExternalLinks(true);
    mDescriptionReadOnly->setWordWrap(true);
    mDescriptionReadOnly->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layoutReadOnly->addRow(i18n("Description:"), mDescriptionReadOnly);
}

ChannelInfoWidget::~ChannelInfoWidget()
{
}

void ChannelInfoWidget::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom->canBeModify()) {
        mStackedWidget->setCurrentWidget(mEditableChannel);
        updateEditableChannelInfo();
        connectEditableWidget();
    } else {
        mStackedWidget->setCurrentWidget(mReadOnlyChannel);
        updateReadOnlyChannelInfo();
        connectReadOnlyWidget();
    }
}

void ChannelInfoWidget::updateReadOnlyChannelInfo()
{
    mNameReadOnly->setText(mRoom->name());
    mCommentReadOnly->setText(mRoom->topic());
    mAnnouncementReadOnly->setText(mRoom->announcement());
    mDescriptionReadOnly->setText(mRoom->description());
}

void ChannelInfoWidget::updateEditableChannelInfo()
{
    mName->setText(mRoom->name());
    mComment->setText(mRoom->topic());
    mAnnouncement->setText(mRoom->announcement());
    mDescription->setText(mRoom->description());
    mReadOnly->setChecked(mRoom->readOnly());
    mArchive->setChecked(mRoom->archived());
    mPrivate->setChecked(mRoom->channelType() == QStringLiteral("p"));
    mEncrypted->setVisible(mRoom->encryptedEnabled());
    mEncrypted->setChecked(mRoom->encrypted());
    mEncryptedLabel->setVisible(mRoom->encryptedEnabled());
    joinCodeChanged();
}

void ChannelInfoWidget::joinCodeChanged()
{
    mPasswordLineEdit->lineEdit()->setPlaceholderText(mRoom->joinCodeRequired() ? i18n("This Room has a password") : i18n("Add password"));
}

void ChannelInfoWidget::connectReadOnlyWidget()
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
}

void ChannelInfoWidget::connectEditableWidget()
{
    connect(mRoom, &Room::announcementChanged, this, [this]() {
        mAnnouncement->setText(mRoom->announcement());
    });
    connect(mRoom, &Room::topicChanged, this, [this]() {
        mComment->setText(mRoom->topic());
    });
    connect(mRoom, &Room::nameChanged, this, [this]() {
        mName->setText(mRoom->name());
    });
    connect(mRoom, &Room::descriptionChanged, this, [this]() {
        mDescription->setText(mRoom->description());
    });
    connect(mRoom, &Room::readOnlyChanged, this, [this]() {
        mReadOnly->setChecked(mRoom->readOnly());
    });
    connect(mRoom, &Room::archivedChanged, this, [this]() {
        mArchive->setChecked(mRoom->archived());
    });
    connect(mRoom, &Room::joinCodeRequiredChanged, this, [this]() {
        joinCodeChanged();
    });
    connect(mRoom, &Room::channelTypeChanged, this, [this]() {
        mPrivate->setChecked(mRoom->channelType() == QStringLiteral("p"));
    });
    //TODO react when we change settings
    connect(mReadOnly, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::ReadOnly, checked, mRoom->channelType());
    });
    connect(mArchive, &QCheckBox::clicked, this, [this](bool checked) {
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, checked ? i18n("Do you want to archive this room?") : i18n("Do you want to unarchive this room?"), i18n("Archive room"))) {
            Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::Archive, checked, mRoom->channelType());
        }
    });
    connect(mPrivate, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeChannelSettings(mRoom->roomId(), RocketChatAccount::RoomType, checked, mRoom->channelType());
    });
}

ChangeTextWidget::ChangeTextWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mLabel = new QLabel(this);
    mLabel->setObjectName(QStringLiteral("mLabel"));
    mLabel->setWordWrap(true);
    mLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mLabel->setOpenExternalLinks(true);
    mLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mainLayout->addWidget(mLabel);
    mChangeTextToolButton = new QToolButton(this);
    mChangeTextToolButton->setIcon(QIcon::fromTheme(QStringLiteral("document-edit")));
    mChangeTextToolButton->setObjectName(QStringLiteral("mChangeTextToolButton"));
    mainLayout->addWidget(mChangeTextToolButton);
    connect(mChangeTextToolButton, &QToolButton::clicked, this, [this]() {
        //Convert html to text. Otherwise we will have html tag
        QString text = mLabel->text();
        QTextDocument doc;
        doc.setHtml(text);
        text = doc.toPlainText();
        const QString result = QInputDialog::getText(this, i18n("Change Text"), mLabelText, QLineEdit::Normal, text);
        if (!result.trimmed().isEmpty()) {
            if (result != text) {
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

void ChangeTextWidget::setLabelText(const QString &str)
{
    mLabelText = str;
}
