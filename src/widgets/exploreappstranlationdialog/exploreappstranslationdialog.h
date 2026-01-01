/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "downloadappslanguages/downloadappslanguagesinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ExploreAppsTranslationWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreAppsTranslationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExploreAppsTranslationDialog(QWidget *parent = nullptr);
    ~ExploreAppsTranslationDialog() override;

    void setAppsLanguagesInfoMap(const QMap<QString, DownloadAppsLanguagesInfo> &map);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ExploreAppsTranslationWidget *const mExploreAppsTranslationWidget;
};
