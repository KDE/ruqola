/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class SelectedMessageBackgroundAnimationLabel : public QWidget
{
    Q_OBJECT
public:
    explicit SelectedMessageBackgroundAnimationLabel(QWidget *parent = nullptr);
    ~SelectedMessageBackgroundAnimationLabel() override;
    void setBackgroundColor(const QColor &col);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor mBackgroundColor;
};

class SelectedMessageBackgroundAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectedMessageBackgroundAnimationWidget(QWidget *parent = nullptr);
    ~SelectedMessageBackgroundAnimationWidget() override;

private:
    SelectedMessageBackgroundAnimationLabel *const mLabel;
};
