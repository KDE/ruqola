/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class QLineEdit;
class MyAccountPersonalAccessTokenTreeView;
class PersonalAccessTokenInfosModel;
class PersonalAccessTokenInfosFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountPersonalAccessTokenConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountPersonalAccessTokenConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountPersonalAccessTokenConfigureWidget() override;

    void initialize();

private:
    void slotCreateToken();
    void slotRemoveToken(const QString &tokenName);
    void slotRegenerateToken(const QString &tokenName);
    void slotTextChanged(const QString &str);
    RocketChatAccount *const mRocketChatAccount;
    QLineEdit *const mSearchLineWidget;
    MyAccountPersonalAccessTokenTreeView *const mPersonalAccessTokenTreeView;
    PersonalAccessTokenInfosModel *const mPersonalAccessTokenModel;
    PersonalAccessTokenInfosFilterProxyModel *const mPersonalAccessTokenFilterProxyModel;
};
