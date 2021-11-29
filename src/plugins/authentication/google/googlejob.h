/*
 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018 Veluri Mithun <velurimithun38@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 *
 */

#pragma once

#include "o2/o0baseauth.h"
#include "o2/o2google.h"
#include <QObject>

class GoogleJob : public QObject
{
    Q_OBJECT

public:
    explicit GoogleJob(QObject *parent = nullptr);
    ~GoogleJob() override;
    void getDataFromJson();

Q_SIGNALS:
    void extraTokensReady(const QVariantMap &extraTokens);
    void linkingFailed(const QString &msg = QString());
    void linkingSucceeded();

    void loginMethodCalled(const QString &credentialToken, const QString &credentialSecret);

public Q_SLOTS:
    void doOAuth(O2::GrantFlow grantFlowType);
    void validateToken();

private:
    void onLinkedChanged();
    void onLinkingSucceeded();
    void onOpenBrowser(const QUrl &url);
    void onCloseBrowser();
    void onFinished();

    void OAuthLoginMethodParameter();
    O2Google *mO2Google = nullptr;
    QString m_clientID;
    QString m_clientSecret;
    QString m_authUri;
    QString m_tokenUri;
    QString m_accessToken;
    bool m_isValidToken = false;
};

