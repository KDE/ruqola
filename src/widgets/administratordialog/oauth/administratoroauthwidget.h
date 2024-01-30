/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class QLineEdit;
class OauthTreeView;
class AdminOauthModel;
class AdministratorOauthFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorOauthWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorOauthWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorOauthWidget() override;

    void initialize();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotListOauthDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveOauth(const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOauthAppAdded(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOauthAppUpdated(const QJsonObject &obj);
    RocketChatAccount *const mRocketChatAccount;
    QLineEdit *const mSearchLineWidget;
    OauthTreeView *const mOauthTreeWidget;
    AdminOauthModel *const mAdminOauthModel;
    AdministratorOauthFilterProxyModel *mAdminInviteFilterProxyModel = nullptr;
};
