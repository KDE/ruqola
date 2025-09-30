/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "aiactionsplugintoolinterface.h"
#include "aiactionsplugin_debug.h"
#include <KLocalizedString>
#include <QMenu>

AiActionsPluginToolInterface::AiActionsPluginToolInterface(QObject *parent)
    : PluginToolInterface{parent}
{
}

AiActionsPluginToolInterface::~AiActionsPluginToolInterface() = default;

void AiActionsPluginToolInterface::activateTool()
{
    switch (mActionAi) {
    case ActionAi::Unknown:
        qCWarning(RUQOLA_AUTOGENERATETEXT_AIACTIONS_LOG) << "ActionAi::Unknown it's a bug";
        break;
    case ActionAi::SummarizeUnreads:
        // TODO
        break;
    }
}

QMenu *AiActionsPluginToolInterface::menu(QWidget *parentWidget) const
{
    auto menu = new QMenu(parentWidget);
    QAction *act = menu->addAction(i18n("Summarize Unreads"));
    connect(act, &QAction::toggled, this, &AiActionsPluginToolInterface::slotSummarize);
    return menu;
}

void AiActionsPluginToolInterface::slotSummarize()
{
    mActionAi = ActionAi::SummarizeUnreads;
    Q_EMIT activateRequested();
}

#include "moc_aiactionsplugintoolinterface.cpp"
