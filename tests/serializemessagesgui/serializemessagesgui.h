/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class SerializeMessageWidget;
class QPlainTextEdit;
class SerializeMessagesGui : public QWidget
{
    Q_OBJECT
public:
    explicit SerializeMessagesGui(QWidget *parent = nullptr);
    ~SerializeMessagesGui() override;

private:
    SerializeMessageWidget *const mOriginal;
    SerializeMessageWidget *const mSerialize;
    QPlainTextEdit *const mDiffMessage;
};
