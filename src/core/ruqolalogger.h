/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "abstractlogger.h"

#include <QFile>

class RuqolaLogger : public RocketChatRestApi::AbstractLogger
{
public:
    explicit RuqolaLogger(const QString &accountName = QString());
    ~RuqolaLogger() override;
    void dataSent(const QByteArray &data) override;
    void dataSent(DataType type, const QByteArray &label, const QByteArray &data) override;
    void dataReceived(const QByteArray &data) override;

private:
    Q_DISABLE_COPY(RuqolaLogger)
    QFile mLoggerFile;
    QFile mRestApiLogFile;
    quint64 mIdentifier = 0;
};
