/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogeneratetextplugintoolinterface.h"
#include "ruqola.h"
AutoGenerateTextPluginToolInterface::AutoGenerateTextPluginToolInterface(QObject *parent)
    : PluginToolInterface{parent}
{
}

AutoGenerateTextPluginToolInterface::~AutoGenerateTextPluginToolInterface() = default;

void AutoGenerateTextPluginToolInterface::activateTool()
{
    if (!mQuickAskDialog) {
        mQuickAskDialog = new TextAutoGenerateText::TextAutoGenerateQuickAskDialog(Ruqola::self()->textAutoGenerateManager(), nullptr);
        mQuickAskDialog->setAttribute(Qt::WA_DeleteOnClose);
        mQuickAskDialog->show();
    } else {
        mQuickAskDialog->raise();
        mQuickAskDialog->activateWindow();
    }
}

#include "moc_autogeneratetextplugintoolinterface.cpp"
