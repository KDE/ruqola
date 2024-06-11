/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistview.h"
#include "applicationssettingsdelegate.h"

ApplicationsSettingsListView::ApplicationsSettingsListView(QWidget *parent)
    : QTreeView(parent)
    , mApplicationsSettingsListDelegate(new ApplicationsSettingsDelegate(this))
{
    mApplicationsSettingsListDelegate->setObjectName(QStringLiteral("mApplicationsSettingsListDelegate"));
    setItemDelegate(mApplicationsSettingsListDelegate);

    setHeaderHidden(true);
    setRootIsDecorated(false);
    // setUniformRowHeights(true);
    setItemsExpandable(false);
    setIndentation(0);
}

ApplicationsSettingsListView::~ApplicationsSettingsListView() = default;

#include "moc_applicationssettingslistview.cpp"
