/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef EXPORTMESSAGESWIDGET_H
#define EXPORTMESSAGESWIDGET_H

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

    Q_REQUIRED_RESULT RocketChatRestApi::RoomsExportJob::RoomsExportInfo roomExportInfo() const;

Q_SIGNALS:
    void updateOkButton(bool state);

private:
    void fillFormat();
    QDateEdit *const mFromDate;
    QDateEdit *const mToDate;
    QComboBox *const mFormat;
};

#endif // EXPORTMESSAGESWIDGET_H
