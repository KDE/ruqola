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

#ifndef INVITEUSERSDIALOG_H
#define INVITEUSERSDIALOG_H

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class InviteUsersWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT InviteUsersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InviteUsersDialog(QWidget *parent = nullptr);
    ~InviteUsersDialog() override;
    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);
    void generateLink();

private:
    void readConfig();
    void writeConfig();
    InviteUsersWidget *const mInviteUsersWidget;
};

#endif // INVITEUSERSDIALOG_H
