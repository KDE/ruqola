/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <QWidget>
class NeedUpdateVersionWidget;
class NeedUpdateVersionMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NeedUpdateVersionMainWidget(QWidget *parent = nullptr);
    ~NeedUpdateVersionMainWidget() override;

private:
    void checkVersion();
    NeedUpdateVersionWidget *const mNeedUpdateVersionWidget;
};
