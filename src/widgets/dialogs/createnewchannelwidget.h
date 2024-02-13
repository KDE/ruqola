/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class ChannelNameValidLineWidget;
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

    [[nodiscard]] QString channelName() const;
    [[nodiscard]] QStringList members(bool userId = true) const;
    [[nodiscard]] bool encryptedRoom() const;
    [[nodiscard]] bool privateChannel() const;
    [[nodiscard]] bool broadCast() const;
    [[nodiscard]] bool readOnly() const;
    [[nodiscard]] QString topic() const;

    void setFeatures(CreateNewChannelWidget::Features features);
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotChangeOkButtonEnabled(bool state);
    ChannelNameValidLineWidget *const mChannelName;
    QLineEdit *const mTopicLineEdit;
    AddUsersWidget *const mUsers;
    QCheckBox *const mReadOnly;
    QCheckBox *const mBroadcast;
    QCheckBox *const mPrivate;
    QCheckBox *const mEncryptedRoom;
    QFormLayout *const mMainLayout;
};
