/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagelineextratoolbutton.h"
#include "actionbuttons/actionbuttonsmanager.h"
#include "actionbuttonsgenerator.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QMenu>
using namespace Qt::Literals::StringLiterals;
MessageLineExtraToolButton::MessageLineExtraToolButton(QWidget *parent)
    : QToolButton(parent)
    , mActionButtonsGenerator(new ActionButtonsGenerator(this))
    , mMenu(new QMenu)
{
    setIcon(QIcon::fromTheme(u"list-add"_s));
    setToolTip(i18nc("@info:tooltip", "More Actions"));
    setVisible(false);
    setAutoRaise(true);
    setPopupMode(QToolButton::InstantPopup);
    setMenu(mMenu);
    connect(mActionButtonsGenerator, &ActionButtonsGenerator::uiInteractionRequested, this, &MessageLineExtraToolButton::uiInteractionRequested);
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
    slotActionButtonChanged();
}

void MessageLineExtraToolButton::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

void MessageLineExtraToolButton::slotActionButtonChanged()
{
    if (mCurrentRocketChatAccount) {
        ActionButton::FilterActionInfo filterInfo;
        filterInfo.buttonContext = ActionButton::ButtonContext::MessageBoxAction;
        const QList<ActionButton> actionButtons = mCurrentRocketChatAccount->actionButtonsManager()->actionButtonsFromFilterActionInfo(filterInfo);
        mActionButtonsGenerator->generateMessageBoxActionButtons(actionButtons, menu(), mRoomId);
        setVisible(!menu()->isEmpty());
    }
}

#include "moc_messagelineextratoolbutton.cpp"
