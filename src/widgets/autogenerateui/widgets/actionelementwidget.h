/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <QWidget>
class QLabel;
class QHBoxLayout;
class ActionElementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ActionElementWidget(QWidget *parent = nullptr);
    ~ActionElementWidget() override;

    void setErrorMessages(const QString &msg);

    void clearError();

private:
    QLabel *const mErrorLabel;
    QHBoxLayout *const mLayout;
};
