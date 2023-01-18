/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ServerErrorInfoMessageHistoryWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerErrorInfoMessageHistoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ServerErrorInfoMessageHistoryDialog(QWidget *parent = nullptr);
    ~ServerErrorInfoMessageHistoryDialog() override;

    void addServerList(const QStringList &serverNames);

private:
    void readConfig();
    void writeConfig();
    ServerErrorInfoMessageHistoryWidget *const mServerErrorInfoMessageHistoryWidget;
};
