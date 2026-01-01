/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class SearchTeamWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchTeamDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SearchTeamDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SearchTeamDialog() override;

    [[nodiscard]] const QByteArray &teamId() const;

private:
    SearchTeamWidget *const mSearchTeamWidget;
};
