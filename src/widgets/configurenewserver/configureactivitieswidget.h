/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QListView;
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureActivitiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureActivitiesWidget(QWidget *parent = nullptr);
    ~ConfigureActivitiesWidget() override;

    [[nodiscard]] AccountManager::ActivitySettings activitiesSettings() const;
    void setActivitiesSettings(const AccountManager::ActivitySettings &activitySettings);

private:
    QListView *const mListView;
    QCheckBox *const mEnableActivitiesSupport;
};
