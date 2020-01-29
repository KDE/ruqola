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

#ifndef CREATENEWCHANNELDIALOG_H
#define CREATENEWCHANNELDIALOG_H

#include <QDialog>
#include "libruqolawidgets_private_export.h"
class CreateNewChannelWidget;
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewChannelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateNewChannelDialog(QWidget *parent = nullptr);
    ~CreateNewChannelDialog();
    struct NewChannelInfo {
        QString usersName;
        QString channelName;
        QString password;
        bool readOnly = false;
        bool broadCast = false;
        bool privateChannel = false;
        bool encryptedRoom = false;
    };
    Q_REQUIRED_RESULT NewChannelInfo channelInfo() const;

private:
    void writeConfig();
    void readConfig();
    CreateNewChannelWidget *mCreateNewChannelWidget = nullptr;
    QPushButton *mOkButton = nullptr;
};

#endif // CREATENEWCHANNELDIALOG_H
