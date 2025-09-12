/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagelineextratoolbutton.h"
#include "actionbuttons/actionbuttonsmanager.h"
#include "actionbuttons/actionbuttonutil.h"
#include "actionbuttonsgenerator.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
using namespace Qt::Literals::StringLiterals;
MessageLineExtraToolButton::MessageLineExtraToolButton(QWidget *parent)
    : QToolButton(parent)
    , mActionButtonsGenerator(new ActionButtonsGenerator(this))

{
    setIcon(QIcon::fromTheme(u"list-add"_s));
    setToolTip(i18nc("@info:tooltip", "More Actions"));
    setVisible(false);
    setAutoRaise(true);
    setPopupMode(QToolButton::InstantPopup);
}

MessageLineExtraToolButton::~MessageLineExtraToolButton() = default;

void MessageLineExtraToolButton::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount->actionButtonsManager(),
                   &ActionButtonsManager::actionButtonsChanged,
                   this,
                   &MessageLineExtraToolButton::slotActionButtonChanged);
    }
    mCurrentRocketChatAccount = account;
    mActionButtonsGenerator->setCurrentRocketChatAccount(account);
    if (mCurrentRocketChatAccount) {
        connect(mCurrentRocketChatAccount->actionButtonsManager(),
                &ActionButtonsManager::actionButtonsChanged,
                this,
                &MessageLineExtraToolButton::slotActionButtonChanged);
    }
    mCurrentRocketChatAccount = account;
}

void MessageLineExtraToolButton::slotActionButtonChanged()
{
    // TODO
}

#include "moc_messagelineextratoolbutton.cpp"
