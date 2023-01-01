/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class SearchTeamCompletionLineEdit;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchTeamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchTeamWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SearchTeamWidget() override;

    Q_REQUIRED_RESULT const QString &teamId() const;

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    SearchTeamCompletionLineEdit *const mSearchLine;
};
