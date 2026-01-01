/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "room/plugins/plugintoolinterface.h"
#include <QPointer>
#include <TextAutoGenerateText/TextAutoGenerateQuickAskDialog>
class AutoGenerateTextPluginToolInterface : public PluginToolInterface
{
    Q_OBJECT
public:
    explicit AutoGenerateTextPluginToolInterface(QWidget *parentWidget, QObject *parent = nullptr);
    ~AutoGenerateTextPluginToolInterface() override;
    void activateTool() override;

private:
    QPointer<TextAutoGenerateText::TextAutoGenerateQuickAskDialog> mQuickAskDialog;
};
