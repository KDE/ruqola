/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QListView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CommandPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommandPreviewWidget(QWidget *parent = nullptr);
    ~CommandPreviewWidget() override;

private:
    QListView *const mListView;
};
