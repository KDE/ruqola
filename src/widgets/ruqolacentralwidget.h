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

#ifndef RUQOLACENTRALWIDGET_H
#define RUQOLACENTRALWIDGET_H

#include <QAbstractSocket>
#include <QWidget>
#include "libruqolawidgets_private_export.h"
class QStackedWidget;
class RuqolaMainWidget;
class RuqolaLoginWidget;
class RocketChatAccount;
class RoomWrapper;

class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaCentralWidget(QWidget *parent = nullptr);
    ~RuqolaCentralWidget();
    Q_REQUIRED_RESULT QString roomId() const;

    void setCurrentRocketChatAccount(RocketChatAccount *account);
    Q_REQUIRED_RESULT QString roomType() const;
    RoomWrapper *roomWrapper() const;
Q_SIGNALS:
    void channelSelected();
private:
    void slotLoginStatusChanged();
    void slotJobFailedInfo(const QString &messageError);
    void slotSocketError(QAbstractSocket::SocketError error, const QString &errorString);

    QStackedWidget *mStackedWidget = nullptr;
    RuqolaMainWidget *mRuqolaMainWidget = nullptr;
    RuqolaLoginWidget *mRuqolaLoginWidget = nullptr;
    RocketChatAccount *mCurrentRocketChatAccount = nullptr;
};

#endif // RUQOLACENTRALWIDGET_H
