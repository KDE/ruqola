/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logssettingswidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

LogsSettingsWidget::LogsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mLogExceptionsChannel(new QLineEdit(this))
{
    mLogExceptionsChannel->setObjectName(QStringLiteral("mLogExceptionsChannel"));
    mLogExceptionsChannel->setToolTip(i18n("A channel that will receive all captured exceptions. Leave empty to ignore exceptions."));
    addLineEdit(i18n("Log Exceptions to Channel"), mLogExceptionsChannel, QStringLiteral("Log_Exceptions_to_Channel"));
}

LogsSettingsWidget::~LogsSettingsWidget() = default;

void LogsSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mLogExceptionsChannel, mapSettings, {});
}
