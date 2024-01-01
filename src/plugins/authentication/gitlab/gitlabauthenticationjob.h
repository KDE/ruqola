/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class QOAuth2AuthorizationCodeFlow;
class GitLabAuthenticationJob : public QObject
{
    Q_OBJECT
public:
    struct GitLabInfo {
        QString clientId;
        QString url;
        QString token;
        QString refreshToken;

        [[nodiscard]] bool isValid() const;
    };
    explicit GitLabAuthenticationJob(QObject *parent = nullptr);
    ~GitLabAuthenticationJob() override;

    void start();

    [[nodiscard]] GitLabInfo gitLabInfo() const;
    void setGitLabInfo(const GitLabInfo &newGitLabInfo);

Q_SIGNALS:
    void authenticateDone();

private:
    void doRequest();
    GitLabInfo mGitLabInfo;
    QOAuth2AuthorizationCodeFlow *const mOAuth2;
};
