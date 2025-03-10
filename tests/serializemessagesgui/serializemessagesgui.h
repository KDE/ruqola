/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QPlainTextEdit;
class SerializeMessagesGui : public QWidget
{
    Q_OBJECT
public:
    explicit SerializeMessagesGui(QWidget *parent = nullptr);
    ~SerializeMessagesGui() override;

private:
    QPlainTextEdit *const mPlainTextEdit;
    QPlainTextEdit *const mSerializeTextEdit;
};
