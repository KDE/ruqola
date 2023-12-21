/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class QOAuth2AuthorizationCodeFlow;
class GitHubAuthenticationJob : public QObject
{
    Q_OBJECT
public:
    explicit GitHubAuthenticationJob(QObject *parent = nullptr);
    ~GitHubAuthenticationJob() override;

    void start();

Q_SIGNALS:
    void authenticated();

private:
    void slotAuthenticated();
    QOAuth2AuthorizationCodeFlow *const mOAuth2;
};
