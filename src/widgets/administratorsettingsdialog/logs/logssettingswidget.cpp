/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logssettingswidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

LogsSettingsWidget::LogsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mLogExceptionsChannel(new QLineEdit(this))
    , mLogViewLimit(new QSpinBox(this))
{
    mLogExceptionsChannel->setObjectName(QStringLiteral("mLogExceptionsChannel"));
    mLogExceptionsChannel->setToolTip(i18n("A channel that will receive all captured exceptions. Leave empty to ignore exceptions."));
    addLineEdit(i18n("Log Exceptions to Channel"), mLogExceptionsChannel, QStringLiteral("Log_Exceptions_to_Channel"));

    mLogViewLimit->setObjectName(QStringLiteral("mLogViewLimit"));
    mLogViewLimit->setMaximum(999999);
    addSpinbox(i18n("Log View Limit"), mLogViewLimit, QStringLiteral("Log_View_Limit"));
}

LogsSettingsWidget::~LogsSettingsWidget() = default;

void LogsSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mLogExceptionsChannel, mapSettings, {});
    initializeWidget(mLogViewLimit, mapSettings, {});
}
