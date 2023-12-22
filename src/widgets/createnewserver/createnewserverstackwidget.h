/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QStackedWidget>
class CreateNewServerCheckUrlWidget;
class CreateNewServerWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerStackWidget(QWidget *parent = nullptr);
    ~CreateNewServerStackWidget() override;

    void setExistingAccountName(const QStringList &lst);
    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() const;

    void setAccountInfo(const AccountManager::AccountManagerInfo &info);

Q_SIGNALS:
    void updateOkButton(bool state);
    void authentication(AuthenticationManager::AuthMethodType type);

private:
    CreateNewServerCheckUrlWidget *const mCreateNewServerCheckUrlWidget;
    CreateNewServerWidget *const mCreateNewServerWidget;
};
