/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class InviteTreeView;
class QLineEdit;
class AdminInviteModel;
class DirectoryBaseFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorInvitesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorInvitesWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorInvitesWidget() override;
    void initialize();

private:
    void slotListInviteDone(const QJsonDocument &obj);
    void slotRemoveInvite(const QString &identifier);
    void slotRemoveInviteDone(const QString &identifier);
    void slotTextChanged(const QString &str);
    InviteTreeView *const mInviteTreeView;
    QLineEdit *const mSearchLineWidget;
    AdminInviteModel *const mAdminInviteModel;
    DirectoryBaseFilterProxyModel *mAdminInviteFilterProxyModel = nullptr;
    RocketChatAccount *const mRocketChatAccount;
};
