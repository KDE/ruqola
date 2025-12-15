/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QNetworkReply>

inline static const QList<QNetworkReply::NetworkError> &networkErrorsNeedingReconnect()
{
    // QNetworkReply::NetworkSessionFailedError = connection was broken due to disconnection from the network or failure to start the network
    static const QList<QNetworkReply::NetworkError> sNetworkErrorsNeedingReconnect = {QNetworkReply::NetworkSessionFailedError,
                                                                                      QNetworkReply::HostNotFoundError,
                                                                                      QNetworkReply::TemporaryNetworkFailureError,
                                                                                      QNetworkReply::UnknownNetworkError}; // maybe others need to be added

    return sNetworkErrorsNeedingReconnect;
}
