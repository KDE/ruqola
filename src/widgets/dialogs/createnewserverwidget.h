/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include <QWidget>

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
class QLineEdit;
class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerWidget(QWidget *parent = nullptr);
    ~CreateNewServerWidget() override;

    Q_REQUIRED_RESULT AccountManager::AccountManagerInfo accountInfo();
    void setAccountInfo(const AccountManager::AccountManagerInfo &info);

    void setExistingAccountName(const QStringList &lst);

Q_SIGNALS:
    void updateOkButton(bool state);

private:
    void slotChangeOkButtonEnabled();
    QStringList mNames;

    QLineEdit *const mAccountName;
    QLineEdit *const mServerName;
    QLineEdit *const mUserName;
    KPasswordLineEdit *const mPasswordLineEdit;
    AccountManager::AccountManagerInfo mAccountInfo;
};
