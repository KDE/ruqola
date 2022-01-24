/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthtreeviewtest.h"
#include "administratordialog/oauth/oauthtreeview.h"
#include <QTest>
QTEST_MAIN(OauthTreeViewTest)
OauthTreeViewTest::OauthTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void OauthTreeViewTest::shouldHaveDefaultValues()
{
    OauthTreeView treeView;
    // TODO
}
