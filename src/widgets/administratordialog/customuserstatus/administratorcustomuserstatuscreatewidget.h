/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "user.h"
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class StatusCombobox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomUserStatusCreateWidget : public QWidget
{
    Q_OBJECT
public:
    struct UserStatusInfo {
        QString name;
        User::PresenceStatus statusType;
        Q_REQUIRED_RESULT bool isValid() const
        {
            return !name.isEmpty() && statusType != User::PresenceStatus::Unknown;
        }
    };

    explicit AdministratorCustomUserStatusCreateWidget(QWidget *parent = nullptr);
    ~AdministratorCustomUserStatusCreateWidget() override;

    Q_REQUIRED_RESULT UserStatusInfo userStatusInfo() const;
    void setUserStatusInfo(const AdministratorCustomUserStatusCreateWidget::UserStatusInfo &info);

Q_SIGNALS:
    void updateOkButton(bool state);

private:
    void slotCheckValues();
    QLineEdit *const mName;
    StatusCombobox *const mStatusCombobox;
};

