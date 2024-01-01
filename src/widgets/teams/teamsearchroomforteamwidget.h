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

    void setPlaceholderText(const QString &str);

    [[nodiscard]] QString placeHolderText() const;

    [[nodiscard]] QStringList roomIds() const;
    QStringList names() const;
Q_SIGNALS:
    void textChanged(const QString &str);
    void userListChanged(bool isNotEmpty);

private:
    void slotRemoveRoom(const QString &name);
    void slotAddNewName(const AddTeamRoomCompletionLineEdit::RoomCompletionInfo &info);
    AddTeamRoomCompletionLineEdit *const mSearchRoomLineEdit;
    FlowLayout *const mFlowLayout;
    QMap<QString, ClickableWidget *> mMap;
};
