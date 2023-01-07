/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "dialogs/adduserscompletionlineedit.h"
#include "libruqolawidgets_private_export.h"
#include <QMap>
#include <QWidget>
class FlowLayout;
class ClickableWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AddUsersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddUsersWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AddUsersWidget() override;

    Q_REQUIRED_RESULT QStringList userIds() const;

    void setPlaceholderText(const QString &str);

    Q_REQUIRED_RESULT QString placeHolderText() const;

    Q_REQUIRED_RESULT QStringList userNames() const;
    Q_REQUIRED_RESULT int numberOfUsers() const;

    Q_REQUIRED_RESULT QLineEdit *lineEdit() const;
Q_SIGNALS:
    void textChanged(const QString &str);
    void userListChanged(bool isNotEmpty);

private:
    void slotRemoveUser(const QString &username);
    void slotAddNewName(const AddUsersCompletionLineEdit::UserCompletionInfo &info);
    AddUsersCompletionLineEdit *const mSearchUserLineEdit;
    FlowLayout *const mFlowLayout;
    QMap<QString, ClickableWidget *> mMap;
};
