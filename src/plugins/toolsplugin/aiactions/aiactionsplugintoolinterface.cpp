/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "aiactionsplugintoolinterface.h"
#include "aiactionsplugin_debug.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QMenu>
#include <TextAutoGenerateText/TextAutoGenerateQuickAskDialog>
AiActionsPluginToolInterface::AiActionsPluginToolInterface(QObject *parent)
    : PluginToolInterface{parent}
{
}

AiActionsPluginToolInterface::~AiActionsPluginToolInterface() = default;

void AiActionsPluginToolInterface::activateTool()
{
    qDebug() << " void AiActionsPluginToolInterface::activateTool()";
    switch (mActionAi) {
    case ActionAi::Unknown:
        qCWarning(RUQOLA_AUTOGENERATETEXT_AIACTIONS_LOG) << "ActionAi::Unknown it's a bug";
        break;
    case ActionAi::SummarizeUnreads:
        slotSummarizeUnreadMessages();
        break;
    }
}

QMenu *AiActionsPluginToolInterface::menu(QWidget *parentWidget) const
{
    auto menu = new QMenu(parentWidget);
    QAction *act = menu->addAction(i18n("Summarize Unreads"));
    connect(act, &QAction::triggered, this, &AiActionsPluginToolInterface::slotSummarize);
    return menu;
}

void AiActionsPluginToolInterface::slotSummarize()
{
    mActionAi = ActionAi::SummarizeUnreads;
    Q_EMIT activateRequested();
}

void AiActionsPluginToolInterface::slotSummarizeUnreadMessages()
{
    // TODO get message model info()
    TextAutoGenerateText::TextAutoGenerateQuickAskDialog d(Ruqola::self()->textAutoGenerateManager(), nullptr);
    d.exec();
}

#include "moc_aiactionsplugintoolinterface.cpp"
