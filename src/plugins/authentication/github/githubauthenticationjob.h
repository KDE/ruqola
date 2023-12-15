/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GitHubAuthenticationJob : public QObject
{
    Q_OBJECT
public:
    explicit GitHubAuthenticationJob(QObject *parent = nullptr);
    ~GitHubAuthenticationJob() override;
    void start();
};
