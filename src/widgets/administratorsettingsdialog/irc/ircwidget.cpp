/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircwidget.h"
#include <KLocalizedString>

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

IrcWidget::IrcWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mCacheLimitOutBoundMessage(new QSpinBox(this))
    , mLoginSuccessful(new QLineEdit(this))
{
    mCacheLimitOutBoundMessage->setObjectName(QStringLiteral("mCacheLimitOutBoundMessage"));
    mCacheLimitOutBoundMessage->setToolTip(i18n("The cache limit for outbound message handling."));
    mCacheLimitOutBoundMessage->setMaximum(99999);
    addSpinbox(i18n("The cache limit for outbound message handling."), mCacheLimitOutBoundMessage, QStringLiteral("IRC_Message_Cache_Size"));

    auto regularExpressionsLabel = createBoldLabel(i18n("Regular Expressions"));
    regularExpressionsLabel->setObjectName(QStringLiteral("regularExpressionsLabel"));
    mMainLayout->addWidget(regularExpressionsLabel);

    mLoginSuccessful->setObjectName(QStringLiteral("mLoginSuccessful"));
    mLoginSuccessful->setToolTip(i18n("Output upon a successful connection to the IRC server."));
    addLineEdit(i18n("Login Successful"), mLoginSuccessful, QStringLiteral("IRC_RegEx_successLogin"));
}

IrcWidget::~IrcWidget() = default;

void IrcWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mCacheLimitOutBoundMessage, mapSettings, 200);
    initializeWidget(mLoginSuccessful, mapSettings, QStringLiteral("Welcome to the freenode Internet Relay Chat Network"));
}

#include "moc_ircwidget.cpp"
