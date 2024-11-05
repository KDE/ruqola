/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QTextEdit;
class CMarkTestGui : public QWidget
{
    Q_OBJECT
public:
    explicit CMarkTestGui(QWidget *parent = nullptr);
    ~CMarkTestGui() override;

private:
    QTextEdit *const mTextEdit;
    QTextEdit *const mTextEditResultCMark;
};
