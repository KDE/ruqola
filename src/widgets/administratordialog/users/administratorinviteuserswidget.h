/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorInviteUsersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorInviteUsersWidget(QWidget *parent = nullptr);
    ~AdministratorInviteUsersWidget() override;

    [[nodiscard]] QStringList emails() const;

Q_SIGNALS:
    void updateButtonOk(bool enabled);

private:
    QLineEdit *const mListEmails;
};
