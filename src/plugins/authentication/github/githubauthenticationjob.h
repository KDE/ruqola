/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class QOAuth2AuthorizationCodeFlow;
class GitHubAuthenticationJob : public QObject
{
    Q_OBJECT
public:
    struct GitHubInfo {
        QString url;
        QString token;
        QString refreshToken;
        QString clientId;

        [[nodiscard]] bool isValid() const;
    };
    explicit GitHubAuthenticationJob(QObject *parent = nullptr);
    ~GitHubAuthenticationJob() override;

    void start();

    [[nodiscard]] GitHubInfo gitHubInfo() const;
    void setGitHubInfo(const GitHubInfo &newGitHubInfo);

Q_SIGNALS:
    void authenticateDone();

private:
    void doRequest();
    GitHubInfo mGitHubInfo;
    QOAuth2AuthorizationCodeFlow *const mOAuth2;
};
