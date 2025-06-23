/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircwidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mCacheLimitOutBoundMessage->setObjectName(u"mCacheLimitOutBoundMessage"_s);
    mCacheLimitOutBoundMessage->setToolTip(i18nc("@info:tooltip", "The cache limit for outbound message handling."));
    mCacheLimitOutBoundMessage->setMaximum(99999);
    addSpinbox(i18n("The cache limit for outbound message handling."), mCacheLimitOutBoundMessage, u"IRC_Message_Cache_Size"_s);

    auto regularExpressionsLabel = createBoldLabel(i18n("Regular Expressions"));
    regularExpressionsLabel->setObjectName(u"regularExpressionsLabel"_s);
    mMainLayout->addWidget(regularExpressionsLabel);

    mLoginSuccessful->setObjectName(u"mLoginSuccessful"_s);
    mLoginSuccessful->setToolTip(i18nc("@info:tooltip", "Output upon a successful connection to the IRC server."));
    addLineEdit(i18n("Login Successful"), mLoginSuccessful, u"IRC_RegEx_successLogin"_s);
}

IrcWidget::~IrcWidget() = default;

void IrcWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mCacheLimitOutBoundMessage, mapSettings, 200);
    initializeWidget(mLoginSuccessful, mapSettings, u"Welcome to the freenode Internet Relay Chat Network"_s);
}

#include "moc_ircwidget.cpp"
