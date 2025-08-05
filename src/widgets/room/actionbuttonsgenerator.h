/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "actionbuttons/actionbutton.h"
#include "rocketchataccount.h"
class QAction;
#include <QJsonObject>
#include <QList>
#include <QPointer>
class QMenu;
class ActionButtonsGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ActionButtonsGenerator(QObject *parent = nullptr);
    ~ActionButtonsGenerator() override;

    void generateActionButtons(const QList<ActionButton> &actionButtons, QMenu *menu, const QByteArray &roomId);
    void setCurrentRocketChatAccount(RocketChatAccount *account);
    void clearActionButtons();
Q_SIGNALS:
    void uiInteractionRequested(const QJsonObject &obj);

private:
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QList<QAction *> mListActionButton;
};
