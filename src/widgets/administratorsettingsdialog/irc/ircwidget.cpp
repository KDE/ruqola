/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircwidget.h"
#include <KLocalizedString>

#include <QSpinBox>

IrcWidget::IrcWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mCacheLimitOutBoundMessage(new QSpinBox(this))
{
    mCacheLimitOutBoundMessage->setObjectName(QStringLiteral("mCacheLimitOutBoundMessage"));
    mCacheLimitOutBoundMessage->setToolTip(i18n("The cache limit for outbound message handling."));
    mCacheLimitOutBoundMessage->setMaximum(99999);
    addSpinbox(i18n("The cache limit for outbound message handling."), mCacheLimitOutBoundMessage, QStringLiteral("IRC_Message_Cache_Size"));
}

IrcWidget::~IrcWidget() = default;

void IrcWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mCacheLimitOutBoundMessage, mapSettings, 200);
}
