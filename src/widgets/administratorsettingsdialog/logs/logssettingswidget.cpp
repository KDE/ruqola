/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logssettingswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    , mLogLevel(new QComboBox(this))
    , mLogViewLimit(new QSpinBox(this))
    , mTraceMethodCalls(new QCheckBox(i18nc("@option:check", "Trace method calls"), this))
    , mTraceMethodFilter(new QLineEdit(this))
    , mTraceSubscriptionCalls(new QCheckBox(i18nc("@option:check", "Trace subscription calls"), this))
    , mTraceSubscriptionFilter(new QLineEdit(this))
{
    mLogExceptionsChannel->setObjectName(u"mLogExceptionsChannel"_s);
    mLogExceptionsChannel->setToolTip(i18nc("@info:tooltip", "A channel that will receive all captured exceptions. Leave empty to ignore exceptions."));
    addLineEdit(i18n("Log Exceptions to Channel"), mLogExceptionsChannel, u"Log_Exceptions_to_Channel"_s);

    mLogLevel->setObjectName(u"mLogLevel"_s);
    const QMap<QString, QString> maps = {
        {u"0"_s, i18n("Errors Only")},
        {u"1"_s, i18n("Errors and Information")},
        {u"2"_s, i18n("Errors, Information and Debug")},
    };
    addComboBox(i18n("Log Level"), maps, mLogLevel, u"Log_Level"_s);

    mLogViewLimit->setObjectName(u"mLogViewLimit"_s);
    mLogViewLimit->setMaximum(999999);
    addSpinbox(i18n("Log View Limit"), mLogViewLimit, u"Log_View_Limit"_s);

    mTraceMethodCalls->setObjectName(u"mTraceMethodCalls"_s);
    addCheckBox(mTraceMethodCalls, u"Log_Trace_Methods"_s);

    mTraceMethodFilter->setObjectName(u"mTraceMethodFilter"_s);
    mTraceMethodFilter->setToolTip(
        i18nc("@info:tooltip", "The text here will be evaluated as RegExp (new RegExp('text')). Keep it empty to show trace of every call."));
    addLineEdit(i18n("Trace method filter"), mTraceMethodFilter, u"Log_Trace_Methods_Filter"_s);

    mTraceSubscriptionCalls->setObjectName(u"mTraceSubscriptionCalls"_s);
    addCheckBox(mTraceSubscriptionCalls, u"Log_Trace_Subscriptions"_s);

    mTraceSubscriptionFilter->setObjectName(u"mTraceSubscriptionFilter"_s);
    mTraceSubscriptionFilter->setToolTip(
        i18nc("@info:tooltip", "The text here will be evaluated as RegExp (new RegExp('text')). Keep it empty to show trace of every call."));
    addLineEdit(i18n("Trace method filter"), mTraceSubscriptionFilter, u"Log_Trace_Subscriptions_Filter"_s);
}

LogsSettingsWidget::~LogsSettingsWidget() = default;

void LogsSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mLogExceptionsChannel, mapSettings, {});
    initializeWidget(mLogViewLimit, mapSettings, 1000);
    initializeWidget(mLogLevel, mapSettings, u"0"_s);
    initializeWidget(mTraceMethodCalls, mapSettings, false);
    initializeWidget(mTraceSubscriptionCalls, mapSettings, false);
    initializeWidget(mTraceMethodFilter, mapSettings, {});
    initializeWidget(mTraceSubscriptionFilter, mapSettings, {});
}

#include "moc_logssettingswidget.cpp"
