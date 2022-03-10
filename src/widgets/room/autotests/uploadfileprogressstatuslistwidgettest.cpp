/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfileprogressstatuslistwidgettest.h"
#include "room/uploadfileprogressstatuslistwidget.h"
#include <QTest>
QTEST_MAIN(UploadFileProgressStatusListWidgetTest)
UploadFileProgressStatusListWidgetTest::UploadFileProgressStatusListWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void UploadFileProgressStatusListWidgetTest::shouldHaveDefaultValues()
{
    UploadFileProgressStatusListWidget w;
    // TODO
}
