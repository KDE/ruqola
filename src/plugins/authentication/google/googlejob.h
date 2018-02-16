/*
 * Copyright 2016 Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2018 Veluri Mithun <velurimithun38@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef Google_H
#define Google_H

#include <QObject>
#include "o2/o2google.h"
#include "o2/o0baseauth.h"

class Google : public QObject
{
    Q_OBJECT

public:
    explicit Google(QObject *parent = nullptr);
    void getDataFromJson();

    unsigned oauthLoginJob;
Q_SIGNALS:
    void extraTokensReady(const QVariantMap &extraTokens);
    void linkingFailed();
    void linkingSucceeded();
    void loginMethodCalled();

public Q_SLOTS:
    void doOAuth(O2::GrantFlow grantFlowType);
    void validateToken();

private Q_SLOTS:
    void onLinkedChanged();
    void onLinkingSucceeded();
    void onOpenBrowser(const QUrl &url);
    void onCloseBrowser();
    void onFinished();

    void OAuthLoginMethodParameter();
private:
    O2Google *mO2Google = nullptr;
    QString m_clientID;
    QString m_clientSecret;
    QString m_authUri;
    QString m_tokenUri;
    QString m_accessToken;
    bool m_isValidToken = false;
};

#endif // Google_H
