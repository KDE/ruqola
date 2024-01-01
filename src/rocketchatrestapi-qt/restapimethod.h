/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiutil.h"
#include <QString>
#include <QUrl>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT RestApiMethod
{
public:
    RestApiMethod();

    [[nodiscard]] QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    [[nodiscard]] QUrl generateUrl(RestApiUtil::RestApiUrlType type, const QString &urlExtension = QString(), bool useV1Extension = true);

private:
    QString mServerUrl;
};
}
