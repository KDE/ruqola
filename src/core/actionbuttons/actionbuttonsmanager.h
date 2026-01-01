/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "actionbutton.h"
#include "libruqolacore_export.h"
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT ActionButtonsManager : public QObject
{
    Q_OBJECT
public:
    explicit ActionButtonsManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~ActionButtonsManager() override;

    [[nodiscard]] QList<ActionButton> actionButtons() const;
    void setActionButtons(const QList<ActionButton> &newActionButtons);

    void parseActionButtons(const QJsonArray &array);

    [[nodiscard]] QList<ActionButton> actionButtonsFromFilterActionInfo(const ActionButton::FilterActionInfo &filterInfo) const;

    void fetchActionButtons();

Q_SIGNALS:
    void actionButtonsChanged();

private:
    QList<ActionButton> mActionButtons;
    RocketChatAccount *const mRocketChatAccount;
};
