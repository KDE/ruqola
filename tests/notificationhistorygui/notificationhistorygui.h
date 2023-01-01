/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QTextEdit;
class NotificationHistoryGui : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationHistoryGui(QWidget *parent = nullptr);
    ~NotificationHistoryGui() override = default;

private:
    void slotSendNotification();
    QTextEdit *const mTextEdit;
};
