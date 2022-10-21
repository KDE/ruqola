/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logssettingswidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

LogsSettingsWidget::LogsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mLogExceptionsChannel(new QLineEdit(this))
    , mLogViewLimit(new QSpinBox(this))
    , mLogLevel(new QComboBox(this))
    , mTraceMethodCalls(new QCheckBox(i18n("Trace method calls"), this))
{
    mLogExceptionsChannel->setObjectName(QStringLiteral("mLogExceptionsChannel"));
    mLogExceptionsChannel->setToolTip(i18n("A channel that will receive all captured exceptions. Leave empty to ignore exceptions."));
    addLineEdit(i18n("Log Exceptions to Channel"), mLogExceptionsChannel, QStringLiteral("Log_Exceptions_to_Channel"));

    mLogLevel->setObjectName(QStringLiteral("mLogLevel"));
    const QMap<QString, QString> maps = {
        {QStringLiteral("0"), i18n("Errors Only")},
        {QStringLiteral("1"), i18n("Errors and Information")},
        {QStringLiteral("2"), i18n("Errors, Information and Debug")},
    };
    addComboBox(i18n("Log Level"), maps, mLogLevel, QStringLiteral("Log_Level"));

    mLogViewLimit->setObjectName(QStringLiteral("mLogViewLimit"));
    mLogViewLimit->setMaximum(999999);
    addSpinbox(i18n("Log View Limit"), mLogViewLimit, QStringLiteral("Log_View_Limit"));

    mTraceMethodCalls->setObjectName(QStringLiteral("mTraceMethodCalls"));
    mMainLayout->addWidget(mTraceMethodCalls);
    connectCheckBox(mTraceMethodCalls, QStringLiteral("Log_Trace_Methods"));
}

LogsSettingsWidget::~LogsSettingsWidget() = default;

void LogsSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mLogExceptionsChannel, mapSettings, {});
    initializeWidget(mLogViewLimit, mapSettings, 1000);
    initializeWidget(mLogLevel, mapSettings, QStringLiteral("0"));
    initializeWidget(mTraceMethodCalls, mapSettings, false);
}
