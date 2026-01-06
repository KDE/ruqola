/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class TimeStampInMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimeStampInMessagesWidget(QWidget *parent = nullptr);
    ~TimeStampInMessagesWidget() override;
};
