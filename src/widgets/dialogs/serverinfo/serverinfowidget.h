/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include <QWidget>
class QLabel;
class ServerConfigInfo;
class LIBRUQOLAWIDGETS_EXPORT ServerInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ServerInfoWidget(QWidget *parent = nullptr);
    ~ServerInfoWidget() override;
    void setServerConfigInfo(ServerConfigInfo *info);

private:
    QLabel *const mAccountName;
    QLabel *const mUserName;
    QLabel *const mServerVersion;
    QLabel *const mServerUrl;
};
