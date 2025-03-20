/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuimanager.h"

AutoGenerateInteractionUiManager::AutoGenerateInteractionUiManager(QObject *parent)
    : QObject{parent}
{
}

AutoGenerateInteractionUiManager::~AutoGenerateInteractionUiManager() = default;

QWidget *AutoGenerateInteractionUiManager::parentWidget() const
{
    return mParentWidget;
}

void AutoGenerateInteractionUiManager::setParentWidget(QWidget *newParentWidget)
{
    mParentWidget = newParentWidget;
}

void AutoGenerateInteractionUiManager::addNewUi(const QJsonObject &obj)
{
    mListJsonObject.append(obj);
}

#include "moc_autogenerateinteractionuimanager.cpp"
