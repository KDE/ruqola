/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "dialogs/uploadfiledialog.h"
#include "room/plugins/plugintoolinterface.h"

class GrabScreenPluginToolInterface : public PluginToolInterface
{
public:
    explicit GrabScreenPluginToolInterface(QWidget *parentWidget, QObject *parent = nullptr);
    ~GrabScreenPluginToolInterface() override;
    void activateTool() override;

private:
    void sendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo);
};
