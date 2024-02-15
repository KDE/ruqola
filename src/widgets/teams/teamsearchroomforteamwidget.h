/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "addteamroomcompletionlineedit.h"
#include "libruqolawidgets_private_export.h"
#include <QMap>
#include <QWidget>
class FlowLayout;
class ClickableWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamSearchRoomForTeamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeamSearchRoomForTeamWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~TeamSearchRoomForTeamWidget() override;

    [[nodiscard]] QStringList roomIds() const;

Q_SIGNALS:
    void textChanged(const QString &str);
    void userListChanged(bool isNotEmpty);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveRoom(const QString &name);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddNewName(const AddTeamRoomCompletionLineEdit::RoomCompletionInfo &info);
    AddTeamRoomCompletionLineEdit *const mSearchRoomLineEdit;
    FlowLayout *const mFlowLayout;
    QMap<QString, ClickableWidget *> mMap;
};
