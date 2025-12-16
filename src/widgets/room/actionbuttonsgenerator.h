/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "actionbuttons/actionbutton.h"
#include <QList>
#include <QPointer>
class QMenu;
class QAction;
class QJsonObject;
class RocketChatAccount;
class ActionButtonsGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ActionButtonsGenerator(QObject *parent = nullptr);
    ~ActionButtonsGenerator() override;

    void generateMessageBoxActionButtons(const QList<ActionButton> &actionButtons, QMenu *menu, const QByteArray &roomId);
    void generateRoomActionButtons(const QList<ActionButton> &actionButtons, QMenu *menu, const QByteArray &roomId);
    void generateMessageActionButtons(const QList<ActionButton> &actionButtons, QMenu *menu, const QByteArray &roomId, const QByteArray &messageId);
    void setCurrentRocketChatAccount(RocketChatAccount *account);
    void clearActionButtons();
Q_SIGNALS:
    void uiInteractionRequested(const QJsonObject &obj);

private:
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QList<QAction *> mListActionButton;
};
