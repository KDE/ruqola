/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiutil.h"
#include <QString>
#include <QUrl>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT RestApiMethod
{
public:
    RestApiMethod();

    Q_REQUIRED_RESULT QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    Q_REQUIRED_RESULT QUrl generateUrl(RestApiUtil::RestApiUrlType type, const QString &urlExtension = QString(), bool useV1Extension = true);

private:
    QString mServerUrl;
};
}
