/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttonsmanager.h"
#include <QJsonArray>
#include <QJsonObject>

ActionButtonsManager::ActionButtonsManager(QObject *parent)
    : QObject{parent}
{
}

ActionButtonsManager::~ActionButtonsManager() = default;

QList<ActionButton> ActionButtonsManager::actionButtons() const
{
    return mActionButtons;
}

void ActionButtonsManager::setActionButtons(const QList<ActionButton> &newActionButtons)
{
    if (mActionButtons != newActionButtons) {
        mActionButtons = newActionButtons;
        Q_EMIT actionButtonsChanged();
    }
}

void ActionButtonsManager::parseActionButtons(const QJsonArray &array)
{
    QList<ActionButton> buttons;
    for (const auto &r : array) {
        ActionButton act;
        act.parseActionButton(r.toObject());
        buttons.append(act);
    }
    setActionButtons(std::move(buttons));
}

#include "moc_actionbuttonsmanager.cpp"
