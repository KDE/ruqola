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
