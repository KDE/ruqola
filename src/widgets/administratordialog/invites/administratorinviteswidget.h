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

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class InviteTreeWidget;
class KTreeWidgetSearchLineWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorInvitesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorInvitesWidget(QWidget *parent = nullptr);
    ~AdministratorInvitesWidget() override;

private:
    // TODO
    // QJsonDocument([{"_id":"D2F6of","_updatedAt":"2021-04-07T06:49:04.571Z","createdAt":"2021-04-07T06:49:04.571Z","days":1,"expires":"2021-04-08T06:49:04.571Z","maxUses":25,"rid":"n2GWePY4zjG48g7qA","userId":"H7Q9djXQ4iShzD9T2","uses":0}])
    struct InviteInfo {
        QString identifier;
        QString roomId;
        QString userId;
        int maxUses = -1;
        int uses = -1;
    };
    void initialize();
    void slotListInviteDone(const QJsonDocument &obj);
    void slotRemoveInvite(const QString &identifier);
    void slotRemoveInviteDone();
    InviteTreeWidget *const mInviteTreeWidget;
    KTreeWidgetSearchLineWidget *const mSearchLineWidget;
};
