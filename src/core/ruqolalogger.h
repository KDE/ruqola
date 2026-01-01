/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "abstractlogger.h"
#include "libruqolacore_export.h"
#include <QFile>

class LIBRUQOLACORE_EXPORT RuqolaLogger : public RocketChatRestApi::AbstractLogger
{
    Q_OBJECT
public:
    explicit RuqolaLogger(const QString &accountName = QString(), QObject *parent = nullptr);
    ~RuqolaLogger() override;
    void dataSent(const QByteArray &data) override;
    void dataSent(DataType type, const QByteArray &label, const QByteArray &data) override;
    void dataReceived(const QByteArray &data) override;
    void dataLoadFromDatabase(const QByteArray &data) override;
    void dataSaveFromDatabase(const QByteArray &data) override;

    [[nodiscard]] QString loggerFilePath() const;
    [[nodiscard]] QString restApiLoggerFilePath() const;

    [[nodiscard]] QString databaseLogFilePath() const;

private:
    Q_DISABLE_COPY(RuqolaLogger)
    QFile mLoggerFile;
    QFile mRestApiLogFile;
    QFile mDatabaseLogFile;
    quint64 mIdentifier = 0;
};
