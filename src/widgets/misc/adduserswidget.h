/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QStringList userIds() const;

    void setPlaceholderText(const QString &str);

    [[nodiscard]] QString placeHolderText() const;

    [[nodiscard]] QStringList userNames() const;
    [[nodiscard]] int numberOfUsers() const;

    [[nodiscard]] QLineEdit *lineEdit() const;
Q_SIGNALS:
    void textChanged(const QString &str);
    void userListChanged(bool isNotEmpty);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveUser(const QString &username);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddNewName(const AddUsersCompletionLineEdit::UserCompletionInfo &info);
    AddUsersCompletionLineEdit *const mSearchUserLineEdit;
    FlowLayout *const mFlowLayout;
    QMap<QString, ClickableWidget *> mMap;
};
