/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

class ServerErrorInfoMessageHistoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ServerErrorInfoMessageHistoryDialog(QWidget *parent = nullptr);
    ~ServerErrorInfoMessageHistoryDialog() override;
};
