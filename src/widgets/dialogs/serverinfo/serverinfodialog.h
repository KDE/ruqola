/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ServerInfoWidget;
class ServerConfigInfo;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ServerInfoDialog(QWidget *parent = nullptr);
    ~ServerInfoDialog() override;
    void setServerConfigInfo(ServerConfigInfo *info);

private:
    ServerInfoWidget *const mServerInfoWidget;
};
