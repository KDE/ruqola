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

#ifndef CUSTOMUSERSTATUSTREEWIDGET_H
#define CUSTOMUSERSTATUSTREEWIDGET_H

#include <QTreeWidget>
#include "customuserstatus.h"

#include "libruqolawidgets_private_export.h"
class QTreeWidget;

class CustomUserStatusTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit CustomUserStatusTreeWidgetItem(QTreeWidget *parent = nullptr);
    ~CustomUserStatusTreeWidgetItem();

    Q_REQUIRED_RESULT CustomUserStatus userStatus() const;
    void setUserStatus(const CustomUserStatus &userStatus);

private:
    CustomUserStatus mUserStatus;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT CustomUserStatusTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    enum UserStatusColumn {
        Name = 0,
        Presence,
        Identifier,
    };
    explicit CustomUserStatusTreeWidget(QWidget *parent = nullptr);
    ~CustomUserStatusTreeWidget();

private:
    void slotCustomContextMenuRequested(const QPoint &pos);
    void addClicked();
    void editClicked();
    void removeClicked();
    void initialize();
};

#endif // CUSTOMUSERSTATUSTREEWIDGET_H
