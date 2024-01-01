/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QTextEdit;
class QPushButton;

class ExpandJson : public QWidget
{
    Q_OBJECT
public:
    explicit ExpandJson(QWidget *parent = nullptr);
    ~ExpandJson() override = default;

private:
    void slotExpandJson();
    QTextEdit *const mInput;
    QTextEdit *const mOutput;
    QPushButton *const mExpandButton;
};
