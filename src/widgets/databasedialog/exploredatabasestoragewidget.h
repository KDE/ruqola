/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QTreeView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseStorageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseStorageWidget(QWidget *parent = nullptr);
    ~ExploreDatabaseStorageWidget() override;

private:
    QTreeView *const mTreeView;
};
