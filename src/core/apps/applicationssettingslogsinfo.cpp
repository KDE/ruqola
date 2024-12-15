/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslogsinfo.h"
#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;

ApplicationsSettingsLogsInfo::ApplicationsSettingsLogsInfo() = default;

ApplicationsSettingsLogsInfo::~ApplicationsSettingsLogsInfo() = default;

void ApplicationsSettingsLogsInfo::parseLogs(const QJsonObject &obj)
{
    mMethod = obj["method"_L1].toString();
    const QJsonArray array = obj["entries"_L1].toArray();
    for (const QJsonValue &current : array) {
        ApplicationsSettingsLogsInfo::LogsArgument log;
        log.parseArguments(current.toObject());
        mArguments.append(log);
    }
}

bool ApplicationsSettingsLogsInfo::operator==(const ApplicationsSettingsLogsInfo &other) const
{
    return mMethod == other.method() && mArguments == other.arguments();
}

QList<ApplicationsSettingsLogsInfo::LogsArgument> ApplicationsSettingsLogsInfo::arguments() const
{
    return mArguments;
}

void ApplicationsSettingsLogsInfo::setArguments(const QList<LogsArgument> &newArguments)
{
    mArguments = newArguments;
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
    return d;
}

void ApplicationsSettingsLogsInfo::LogsArgument::parseArguments(const QJsonObject &obj)
{
    caller = obj["caller"_L1].toString();
    method = obj["method"_L1].toString();
    severity = obj["severity"_L1].toString(); // TODO convert to enum !!!
    // TODO extract args
    //  TODO
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
