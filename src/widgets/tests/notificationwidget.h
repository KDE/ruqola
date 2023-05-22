/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QLineEdit;
class NotificationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationWidget(QWidget *parent = nullptr);
    ~NotificationWidget() override;

private:
    void slotSendNotification();
    QLineEdit *const mLineEdit;
};
