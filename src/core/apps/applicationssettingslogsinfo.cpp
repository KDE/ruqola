/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslogsinfo.h"
#include "utils.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTimeZone>
using namespace Qt::Literals::StringLiterals;

ApplicationsSettingsLogsInfo::ApplicationsSettingsLogsInfo() = default;

ApplicationsSettingsLogsInfo::~ApplicationsSettingsLogsInfo() = default;

void ApplicationsSettingsLogsInfo::parseLogs(const QJsonObject &obj)
{
    mMethod = obj["method"_L1].toString();
    mTotalTime = obj["totalTime"_L1].toInteger();
    mCreatedAt = QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("_createdAt"), obj), QTimeZone::utc());
    const QJsonArray array = obj["entries"_L1].toArray();
    for (const QJsonValue &current : array) {
        ApplicationsSettingsLogsInfo::LogsArgument log;
        log.parseArguments(current.toObject());
        mArguments.append(log);
    }
}

bool ApplicationsSettingsLogsInfo::operator==(const ApplicationsSettingsLogsInfo &other) const
{
    return mMethod == other.method() && mArguments == other.arguments() && mCreatedAt == other.createdAt() && mTotalTime == other.totalTime();
}

QList<ApplicationsSettingsLogsInfo::LogsArgument> ApplicationsSettingsLogsInfo::arguments() const
{
    return mArguments;
}

void ApplicationsSettingsLogsInfo::setArguments(const QList<LogsArgument> &newArguments)
{
    mArguments = newArguments;
}

QDateTime ApplicationsSettingsLogsInfo::createdAt() const
{
    return mCreatedAt;
}

void ApplicationsSettingsLogsInfo::setCreatedAt(const QDateTime &newCreatedAt)
{
    mCreatedAt = newCreatedAt;
}

qint64 ApplicationsSettingsLogsInfo::totalTime() const
{
    return mTotalTime;
}

void ApplicationsSettingsLogsInfo::setTotalTime(qint64 newTotalTime)
{
    mTotalTime = newTotalTime;
}

QString ApplicationsSettingsLogsInfo::method() const
{
    return mMethod;
}

void ApplicationsSettingsLogsInfo::setMethod(const QString &newMethod)
{
    mMethod = newMethod;
}

QDebug operator<<(QDebug d, const ApplicationsSettingsLogsInfo &t)
{
    d.space() << "method" << t.method();
    d.space() << "arguments" << t.arguments();
    d.space() << "createdAt" << t.createdAt();
    d.space() << "totalTime" << t.totalTime();
    return d;
}

void ApplicationsSettingsLogsInfo::LogsArgument::parseArguments(const QJsonObject &obj)
{
    caller = obj["caller"_L1].toString();
    method = obj["method"_L1].toString();
    severity = obj["severity"_L1].toString(); // TODO convert to enum !!!
    const QJsonArray arrayArgs = obj["args"_L1].toArray();
    for (const QJsonValue &current : arrayArgs) {
        if (current.isBool()) {
            args.append(current.toBool() ? QStringLiteral("true") : QStringLiteral("false"));
        } else if (current.isNull()) {
            args.append(QStringLiteral("null"));
        } else {
            args.append(current.toString());
        }
    }
}

bool ApplicationsSettingsLogsInfo::LogsArgument::operator==(const LogsArgument &other) const
{
    return caller == other.caller && method == other.method && severity == other.severity && args == other.args;
}

QDebug operator<<(QDebug d, const ApplicationsSettingsLogsInfo::LogsArgument &arg)
{
    d.space() << "caller" << arg.caller;
    d.space() << "method" << arg.method;
    d.space() << "severity" << arg.severity;
    d.space() << "args" << arg.args;
    return d;
}
