/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "room/plugins/plugintoolinterface.h"
class QMenu;
class AiActionsPluginToolInterface : public PluginToolInterface
{
    Q_OBJECT
public:
    enum class ActionAi : uint8_t {
        Unknown = 0,
        SummarizeUnreads,
        // TODO add more action
    };

    explicit AiActionsPluginToolInterface(QWidget *parentWidget, QObject *parent = nullptr);
    ~AiActionsPluginToolInterface() override;
    void activateTool() override;
    [[nodiscard]] QMenu *menu() const override;

private:
    void slotSummarize();
    void slotSummarizeUnreadMessages();
    ActionAi mActionAi = ActionAi::Unknown;
};
