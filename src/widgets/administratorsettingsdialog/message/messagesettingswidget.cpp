/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagesettingswidget.h"
#include <QCheckBox>
#include <QFormLayout>

#include <KLocalizedString>

MessageSettingsWidget::MessageSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

MessageSettingsWidget::~MessageSettingsWidget() = default;
