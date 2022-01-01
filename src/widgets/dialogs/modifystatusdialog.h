/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "user.h"
#include <QDialog>
class ModifyStatusWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ModifyStatusDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModifyStatusDialog(QWidget *parent = nullptr);
    ~ModifyStatusDialog() override;

    Q_REQUIRED_RESULT User::PresenceStatus status() const;
    void setStatus(User::PresenceStatus status);

    Q_REQUIRED_RESULT QString messageStatus() const;
    void setMessageStatus(const QString &statusStr);

private:
    ModifyStatusWidget *const mModifyStatusWidget;
};

