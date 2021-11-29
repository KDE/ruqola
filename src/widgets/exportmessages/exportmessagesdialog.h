/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "rooms/roomsexportjob.h"
#include <QDialog>
class ExportMessagesWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportMessagesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExportMessagesDialog(QWidget *parent = nullptr);
    ~ExportMessagesDialog() override;

    Q_REQUIRED_RESULT RocketChatRestApi::RoomsExportJob::RoomsExportInfo roomExportInfo() const;

private:
    void readConfig();
    void writeConfig();
    ExportMessagesWidget *const mExportMessagesWidget;
};

