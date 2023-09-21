/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include "user.h"
class QLineEdit;
class StatusCombobox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ModifyStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModifyStatusWidget(QWidget *parent = nullptr);
    ~ModifyStatusWidget() override;

    [[nodiscard]] QString messageStatus() const;
    [[nodiscard]] User::PresenceStatus status() const;

    void setStatus(User::PresenceStatus status);
    void setMessageStatus(const QString &statusStr);

private:
    StatusCombobox *const mStatusCombobox;
    QLineEdit *const mStatusLineEdit;
};
