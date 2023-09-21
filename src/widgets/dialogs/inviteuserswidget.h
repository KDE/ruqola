/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "invite/findorcreateinvitejob.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class QLabel;
class QComboBox;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT InviteUsersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InviteUsersWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~InviteUsersWidget() override;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    void generateLink(int maxUses = 25, int numberOfDays = 1);

private:
    void fillComboBox();
    void slotFindOrCreateInvite(const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &info);
    void slotCopyLink();
    void slotGenerateNewLink();
    QString mRoomId;
    QLineEdit *const mInviteUserLineEdit;
    QLabel *const mExpireDateLabel;
    QComboBox *const mExpirationDays;
    QComboBox *const mMaxUses;
    RocketChatAccount *const mRocketChatAccount;
};
