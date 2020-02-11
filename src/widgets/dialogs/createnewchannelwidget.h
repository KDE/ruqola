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

#ifndef CREATENEWCHANNELWIDGET_H
#define CREATENEWCHANNELWIDGET_H

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QCheckBox;
class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateNewChannelWidget(QWidget *parent = nullptr);
    ~CreateNewChannelWidget() override;

    Q_REQUIRED_RESULT QString channelName() const;
    Q_REQUIRED_RESULT QString users() const;
    Q_REQUIRED_RESULT bool encryptedRoom() const;
    Q_REQUIRED_RESULT bool privateChannel() const;
    Q_REQUIRED_RESULT bool broadCast() const;
    Q_REQUIRED_RESULT bool readOnly() const;
    Q_REQUIRED_RESULT QString password() const;

Q_SIGNALS:
    void updateOkButton(bool state);

private:
    void slotChangeOkButtonEnabled();
    QLineEdit *mChannelName = nullptr;
    QLineEdit *mUsers = nullptr;
    QCheckBox *mReadOnly = nullptr;
    QCheckBox *mBroadcast = nullptr;
    QCheckBox *mPrivate = nullptr;
    QCheckBox *mEncryptedRoom = nullptr;
    KPasswordLineEdit *mPasswordLineEdit = nullptr;
};

#endif // CREATENEWCHANNELWIDGET_H
