/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttonsmanager.h"

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
    mActionButtons = newActionButtons;
}

#include "moc_actionbuttonsmanager.cpp"
