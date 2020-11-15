/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef ADMINISTRATORCUSTOMUSERSTATUSCREATEWIDGET_H
#define ADMINISTRATORCUSTOMUSERSTATUSCREATEWIDGET_H

#include <QWidget>
#include "user.h"

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

#endif // ADMINISTRATORCUSTOMUSERSTATUSCREATEWIDGET_H
