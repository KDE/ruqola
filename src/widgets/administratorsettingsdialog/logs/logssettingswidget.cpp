/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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
    , mTraceMethodFilter(new QLineEdit(this))
    , mTraceSubscriptionCalls(new QCheckBox(i18n("Trace subscription calls"), this))
    , mTraceSubscriptionFilter(new QLineEdit(this))
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

    mTraceMethodFilter->setObjectName(QStringLiteral("mTraceMethodFilter"));
    mTraceMethodFilter->setToolTip(i18n("The text here will be evaluated as RegExp (new RegExp('text')). Keep it empty to show trace of every call."));
    addLineEdit(i18n("Trace method filter"), mTraceMethodFilter, QStringLiteral("Log_Trace_Methods_Filter"));

    mTraceSubscriptionCalls->setObjectName(QStringLiteral("mTraceSubscriptionCalls"));
    mMainLayout->addWidget(mTraceSubscriptionCalls);
    connectCheckBox(mTraceSubscriptionCalls, QStringLiteral("Log_Trace_Subscriptions"));

    mTraceSubscriptionFilter->setObjectName(QStringLiteral("mTraceSubscriptionFilter"));
    mTraceSubscriptionFilter->setToolTip(i18n("The text here will be evaluated as RegExp (new RegExp('text')). Keep it empty to show trace of every call."));
    addLineEdit(i18n("Trace method filter"), mTraceSubscriptionFilter, QStringLiteral("Log_Trace_Subscriptions_Filter"));
}

LogsSettingsWidget::~LogsSettingsWidget() = default;

void LogsSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mLogExceptionsChannel, mapSettings, {});
    initializeWidget(mLogViewLimit, mapSettings, 1000);
    initializeWidget(mLogLevel, mapSettings, QStringLiteral("0"));
    initializeWidget(mTraceMethodCalls, mapSettings, false);
    initializeWidget(mTraceSubscriptionCalls, mapSettings, false);
    initializeWidget(mTraceMethodFilter, mapSettings, {});
    initializeWidget(mTraceSubscriptionFilter, mapSettings, {});
}

#include "moc_logssettingswidget.cpp"
