/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "downloadappslanguages/downloadappslanguagesinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QTreeWidget;
class KTreeWidgetSearchLine;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreAppsTranslationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreAppsTranslationWidget(QWidget *parent = nullptr);
    ~ExploreAppsTranslationWidget() override;

    void setAppsLanguagesInfoMap(const QMap<QString, DownloadAppsLanguagesInfo> &map);

    [[nodiscard]] QByteArray saveState() const;
    void restoreState(const QByteArray &state);

private:
    QTreeWidget *const mTreeWidget;
    KTreeWidgetSearchLine *const mSearchLineEdit;
};
