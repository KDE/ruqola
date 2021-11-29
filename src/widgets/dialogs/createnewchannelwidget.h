/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class ChannelNameValidLineEdit;
class QCheckBox;
class AddUsersWidget;
class QFormLayout;
class QLineEdit;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewChannelWidget : public QWidget
{
    Q_OBJECT
public:
    enum class Feature {
        None = 0,
        BroadCast = 1,
        Encrypted = 2,
    };
    Q_DECLARE_FLAGS(Features, Feature)
    Q_FLAG(Features)

    explicit CreateNewChannelWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateNewChannelWidget() override;

    Q_REQUIRED_RESULT QString channelName() const;
    Q_REQUIRED_RESULT QStringList users() const;
    Q_REQUIRED_RESULT bool encryptedRoom() const;
    Q_REQUIRED_RESULT bool privateChannel() const;
    Q_REQUIRED_RESULT bool broadCast() const;
    Q_REQUIRED_RESULT bool readOnly() const;
    Q_REQUIRED_RESULT QString topic() const;

    void setFeatures(CreateNewChannelWidget::Features features);
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    void slotChangeOkButtonEnabled(bool state);
    ChannelNameValidLineEdit *const mChannelName;
    AddUsersWidget *const mUsers;
    QCheckBox *const mReadOnly;
    QCheckBox *const mBroadcast;
    QCheckBox *const mPrivate;
    QCheckBox *const mEncryptedRoom;
    QLineEdit *const mTopicLineEdit;
    QFormLayout *const mMainLayout;
};

