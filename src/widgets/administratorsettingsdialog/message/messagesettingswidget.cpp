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
    , mAllowMessageEditing(new QCheckBox(i18n("Allow Message Editing"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAllowMessageEditing->setObjectName(QStringLiteral("mEnableE2E"));
    mainLayout->addWidget(mAllowMessageEditing);
    connect(mAllowMessageEditing, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Message_AllowEditing"), checked);
    });
}

MessageSettingsWidget::~MessageSettingsWidget() = default;
