/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QComboBox>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServersComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ServersComboBox(QWidget *parent = nullptr);
    ~ServersComboBox() override;

    void addServerList(const QList<AccountManager::AccountDisplayInfo> &infos);

Q_SIGNALS:
    void accountSelected(const QString &accountName);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSelectAccount(int index);
};
