/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "aiactionsplugintoolinterface.h"
#include <KLocalizedString>
#include <QMenu>

AiActionsPluginToolInterface::AiActionsPluginToolInterface(QObject *parent)
    : PluginToolInterface{parent}
{
}

AiActionsPluginToolInterface::~AiActionsPluginToolInterface() = default;

void AiActionsPluginToolInterface::activateTool()
{
    // TODO
    switch (mActionAi) {
    case ActionAi::Unknown:
        break;
    case ActionAi::SummarizeUnreads:
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
