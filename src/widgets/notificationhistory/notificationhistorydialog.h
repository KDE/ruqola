/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class NotificationHistoryWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NotificationHistoryDialog(QWidget *parent = nullptr);
    ~NotificationHistoryDialog() override;

private:
    NotificationHistoryWidget *const mNotificationHistoryWidget;
};
