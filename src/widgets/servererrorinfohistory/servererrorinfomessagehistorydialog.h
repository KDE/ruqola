/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ServerErrorInfoMessageHistoryWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerErrorInfoMessageHistoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ServerErrorInfoMessageHistoryDialog(QWidget *parent = nullptr);
    ~ServerErrorInfoMessageHistoryDialog() override;

    void addServerList(const QList<AccountManager::AccountDisplayInfo> &info);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ServerErrorInfoMessageHistoryWidget *const mServerErrorInfoMessageHistoryWidget;
};
