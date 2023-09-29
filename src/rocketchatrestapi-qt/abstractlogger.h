/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librocketchatrestapi-qt_export.h"
class QByteArray;

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT AbstractLogger
{
public:
    AbstractLogger();
    virtual ~AbstractLogger();

    enum DataType {
        Unknown = 0,
        DDPType = 1,
        RESTApiType = 2,
    };

    virtual void dataSent(const QByteArray &data) = 0;
    virtual void dataReceived(const QByteArray &data) = 0;
    virtual void dataSent(DataType type, const QByteArray &label, const QByteArray &data) = 0;
};
}
