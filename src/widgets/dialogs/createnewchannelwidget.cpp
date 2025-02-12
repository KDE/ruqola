/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewchannelwidget.h"
#include "channelnamevalidlineedit.h"
#include "channelnamevalidlinewidget.h"
#include "misc/adduserswidget.h"
#include <KAuthorized>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

CreateNewChannelWidget::CreateNewChannelWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mChannelName(new ChannelNameValidLineWidget(account, this))
    , mTopicLineEdit(new QLineEdit(this))
    , mUsers(new AddUsersWidget(account, this))
    , mReadOnly(new QCheckBox(this))
    , mBroadcast(new QCheckBox(this))
    , mPrivate(new QCheckBox(this))
    , mEncryptedRoom(new QCheckBox(this))
    , mFederated(new QCheckBox(this))
    , mMainLayout(new QFormLayout(this))
{
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});

    mChannelName->setObjectName(QStringLiteral("mChannelName"));
    mMainLayout->addRow(i18n("Name:"), mChannelName);

    mTopicLineEdit->setObjectName(QStringLiteral("mTopicLineEdit"));
    mMainLayout->addRow(i18n("Topic:"), mTopicLineEdit);
    KLineEditEventHandler::catchReturnKey(mTopicLineEdit);

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18nc("@info:placeholder", "Invite users…"));
    mMainLayout->addRow(i18n("Users:"), mUsers);

    mReadOnly->setObjectName(QStringLiteral("mReadOnly"));
    mReadOnly->setChecked(false);
    mReadOnly->setToolTip(i18nc("@info:tooltip", "Anyone can send new messages"));
    mMainLayout->addRow(i18n("Read-Only:"), mReadOnly);
    auto label = new QLabel(i18nc("@label:textbox", "Anyone can send new messages"), this);
    connect(mReadOnly, &QCheckBox::clicked, this, [this, label](bool checked) {
        mReadOnly->setToolTip(checked ? i18nc("@info:tooltip", "Only owners can send new messages") : i18nc("@info:tooltip", "Anyone can send new messages"));
        label->setText(checked ? i18nc("@label:textbox", "Only owners can send new messages") : i18nc("@label:textbox", "Anyone can send new messages"));
    });
    mMainLayout->addWidget(label);

    mBroadcast->setObjectName(QStringLiteral("mBroadcast"));
    mBroadcast->setChecked(false);
    mBroadcast->setToolTip(i18nc("@info:tooltip", "Only authorized users can write new messages, but the other users will be able to reply"));
    mMainLayout->addRow(i18n("Broadcast:"), mBroadcast);

    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    mPrivate->setChecked(false);
    mPrivate->setToolTip(i18nc("@info:tooltip", "Only invited people can join"));

    mMainLayout->addRow(i18n("Private Room:"), mPrivate);
    label = new QLabel(i18nc("@label:textbox", "Just invited people can access this channel."), this);
    mMainLayout->addWidget(label);

    mEncryptedRoom->setObjectName(QStringLiteral("mEncryptedRoom"));
    mEncryptedRoom->setChecked(false);
    mEncryptedRoom->setToolTip(
        i18nc("@label:textbox",
              "End-to-end encrypted channel. Search will not work with encrypted channel and notifications may not show the messages content."));

    mMainLayout->addRow(i18n("Encrypted Room:"), mEncryptedRoom);

    mFederated->setObjectName(QStringLiteral("mFederated"));
    mFederated->setChecked(false);
    mMainLayout->addRow(i18n("Federated:"), mFederated);

    connect(mChannelName, &ChannelNameValidLineWidget::channelIsValid, this, &CreateNewChannelWidget::slotChangeOkButtonEnabled);
}

CreateNewChannelWidget::~CreateNewChannelWidget() = default;

void CreateNewChannelWidget::slotChangeOkButtonEnabled(bool state)
{
    Q_EMIT updateOkButton(/*!mChannelName->text().trimmed().isEmpty()*/ state);
}

QString CreateNewChannelWidget::channelName() const
{
    return mChannelName->text().trimmed();
}

QStringList CreateNewChannelWidget::members(bool userId) const
{
    if (userId) {
        QStringList lst;
        for (const QByteArray &b : mUsers->userIds()) {
            lst.append(QString::fromLatin1(b));
        }
        return lst;
    } else {
        return mUsers->userNames();
    }
}

bool CreateNewChannelWidget::readOnly() const
{
    return mReadOnly->isChecked();
}

bool CreateNewChannelWidget::broadCast() const
{
    return mBroadcast->isChecked();
}

bool CreateNewChannelWidget::privateChannel() const
{
    return mPrivate->isChecked();
}

bool CreateNewChannelWidget::encryptedRoom() const
{
    return mEncryptedRoom->isChecked();
}

QString CreateNewChannelWidget::topic() const
{
    return mTopicLineEdit->text();
}

bool CreateNewChannelWidget::federated() const
{
    return mFederated->isChecked();
}

void CreateNewChannelWidget::setFeatures(CreateNewChannelWidget::Features features)
{
    bool visible = features & CreateNewChannelWidget::Feature::BroadCast;

    mBroadcast->setVisible(visible);
    mMainLayout->labelForField(mBroadcast)->setVisible(visible);

    visible = features & CreateNewChannelWidget::Feature::Encrypted;

    mEncryptedRoom->setVisible(visible);
    mMainLayout->labelForField(mEncryptedRoom)->setVisible(visible);

    visible = features & CreateNewChannelWidget::Feature::Federated;
    mFederated->setVisible(visible);
    mMainLayout->labelForField(mFederated)->setVisible(visible);
}

#include "moc_createnewchannelwidget.cpp"
