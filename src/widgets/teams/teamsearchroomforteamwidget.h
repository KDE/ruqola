/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "addteamroomcompletionlineedit.h"
#include "libruqolawidgets_private_export.h"
#include <QMap>
#include <QWidget>
class FlowLayout;
class ClickableWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamSearchRoomForTeamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeamSearchRoomForTeamWidget(QWidget *parent = nullptr);
    ~TeamSearchRoomForTeamWidget() override;

    void setPlaceholderText(const QString &str);

    Q_REQUIRED_RESULT QString placeHolderText() const;

    Q_REQUIRED_RESULT QStringList roomIds() const;
    QStringList names() const;
Q_SIGNALS:
    void textChanged(const QString &str);
    void userListChanged(bool isNotEmpty);

private:
    AddTeamRoomCompletionLineEdit *const mSearchRoomLineEdit;
    void slotRemoveRoom(const QString &name);
    void slotAddNewName(const AddTeamRoomCompletionLineEdit::RoomCompletionInfo &info);
    FlowLayout *const mFlowLayout;
    QMap<QString, ClickableWidget *> mMap;
};
