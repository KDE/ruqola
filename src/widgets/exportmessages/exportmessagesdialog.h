/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    [[nodiscard]] RocketChatRestApi::RoomsExportJob::RoomsExportInfo roomExportInfo() const;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ExportMessagesWidget *const mExportMessagesWidget;
};
