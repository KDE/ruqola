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
    , mAllowMessageDeleting(new QCheckBox(i18n("Allow Message Deleting"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAllowMessageEditing->setObjectName(QStringLiteral("mAllowMessageEditing"));
    mainLayout->addWidget(mAllowMessageEditing);
    connect(mAllowMessageEditing, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Message_AllowEditing"), checked);
    });

    mAllowMessageDeleting->setObjectName(QStringLiteral("mAllowMessageDeleting"));
    mainLayout->addWidget(mAllowMessageDeleting);
    connect(mAllowMessageDeleting, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Message_AllowDeleting"), checked);
    });
}

MessageSettingsWidget::~MessageSettingsWidget() = default;
