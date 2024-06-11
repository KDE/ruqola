/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QTreeView>
class ApplicationsSettingsDelegate;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsListView : public QTreeView
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListView(QWidget *parent = nullptr);
    ~ApplicationsSettingsListView() override;

private:
    ApplicationsSettingsDelegate *const mApplicationsSettingsListDelegate;
};
