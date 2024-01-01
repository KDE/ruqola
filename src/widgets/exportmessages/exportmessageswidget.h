/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include "rooms/roomsexportjob.h"
class QDateEdit;
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExportMessagesWidget(QWidget *parent = nullptr);
    ~ExportMessagesWidget() override;

    [[nodiscard]] RocketChatRestApi::RoomsExportJob::RoomsExportInfo roomExportInfo() const;

    // Q_SIGNALS:
    //    void updateOkButton(bool state);

private:
    void fillFormat();
    QDateEdit *const mFromDate;
    QDateEdit *const mToDate;
    QComboBox *const mFormat;
};
