/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QPlainTextEdit;
class AutoGenerateInteractionGui : public QWidget
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionGui(QWidget *parent = nullptr);
    ~AutoGenerateInteractionGui() override;

private:
    QPlainTextEdit *const mPlainTextEdit;
    QPlainTextEdit *const mSerializeTextEdit;
};
