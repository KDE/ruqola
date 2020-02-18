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

#ifndef CLICKABLEUSERWIDGET_H
#define CLICKABLEUSERWIDGET_H

#include <QLabel>
#include <QWidget>
#include "libruqolawidgets_export.h"

class LIBRUQOLAWIDGETS_EXPORT ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = nullptr);
    ~ClickableLabel();

Q_SIGNALS:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class LIBRUQOLAWIDGETS_EXPORT ClickableUserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClickableUserWidget(const QString &userName, QWidget *parent = nullptr);
    ~ClickableUserWidget();

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);


Q_SIGNALS:
    void removeUser(const QString &username);

private:
    void slotRemoveUser();
    QString mUserName;
    QString mUserId;
    QLabel *mUserLabel = nullptr;
    ClickableLabel *mClickableLabel = nullptr;
};

#endif // CLICKABLEUSERWIDGET_H
