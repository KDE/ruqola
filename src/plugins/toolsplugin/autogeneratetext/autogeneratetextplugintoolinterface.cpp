/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogeneratetextplugintoolinterface.h"
#include "ruqola.h"
AutoGenerateTextPluginToolInterface::AutoGenerateTextPluginToolInterface(QWidget *parentWidget, QObject *parent)
    : PluginToolInterface{parentWidget, parent}
{
}

AutoGenerateTextPluginToolInterface::~AutoGenerateTextPluginToolInterface() = default;

void AutoGenerateTextPluginToolInterface::activateTool()
{
    if (!mQuickAskDialog) {
        mQuickAskDialog = new TextAutoGenerateText::TextAutoGenerateQuickAskDialog(Ruqola::self()->textAutoGenerateManager(), mParentWidget);
        mQuickAskDialog->setAttribute(Qt::WA_DeleteOnClose);
        mQuickAskDialog->show();
    } else {
        mQuickAskDialog->raise();
        mQuickAskDialog->activateWindow();
    }
}

#include "moc_autogeneratetextplugintoolinterface.cpp"
