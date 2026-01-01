/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiutil.h"
#include <QString>
#include <QUrl>
using namespace Qt::Literals::StringLiterals;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RestApiMethod
{
public:
    RestApiMethod();

    [[nodiscard]] QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    [[nodiscard]] QUrl generateUrl(RestApiUtil::RestApiUrlType type,
                                   RestApiUtil::RestApiUrlExtensionType restApiUrlExtensionType = RestApiUtil::RestApiUrlExtensionType::V1,
                                   const QString &urlExtension = QString());

private:
    QString mServerUrl;
};
}
