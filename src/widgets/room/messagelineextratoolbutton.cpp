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
    // TODO use apps/meteor/client/views/room/composer/messageBox/MessageBoxActionsToolbar/hooks/useShareLocationAction.tsx
    // TODO ./apps/meteor/client/views/room/composer/messageBox/MessageBoxActionsToolbar/hooks/useWebdavActions.tsx
    // TODO see apps/meteor/client/views/room/composer/messageBox/MessageBoxActionsToolbar/MessageBoxActionsToolbar.tsx
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

void MessageLineExtraToolButton::setRoomId(const QByteArray &roomId)
{
    if (mRoomId != roomId) {
        mRoomId = roomId;
        slotActionButtonChanged();
    }
}

void MessageLineExtraToolButton::slotActionButtonChanged()
{
    if (mCurrentRocketChatAccount) {
        const ActionButton::FilterActionInfo filterInfo{.buttonContext = ActionButton::ButtonContext::MessageBoxAction, .roles = {}};
        const QList<ActionButton> actionButtons = mCurrentRocketChatAccount->actionButtonsManager()->actionButtonsFromFilterActionInfo(filterInfo);
        // qDebug() << "*************** actionButtons " << actionButtons << "mRoomId  " << mRoomId;
        mActionButtonsGenerator->generateMessageBoxActionButtons(actionButtons, menu(), mRoomId);
        setVisible(!menu()->isEmpty());
    }
}

#include "moc_messagelineextratoolbutton.cpp"
