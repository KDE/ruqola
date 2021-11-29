/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentcomboboxtest.h"
#include "dialogs/showattachmentcombobox.h"
#include <QTest>
QTEST_MAIN(ShowAttachmentComboBoxTest)
ShowAttachmentComboBoxTest::ShowAttachmentComboBoxTest(QObject *parent)
    : QObject(parent)
{
}

void ShowAttachmentComboBoxTest::shouldHaveDefaultValues()
{
    ShowAttachmentComboBox w;
    QVERIFY(w.count() != 0);
}
