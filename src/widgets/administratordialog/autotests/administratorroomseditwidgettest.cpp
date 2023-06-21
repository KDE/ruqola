/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomseditwidgettest.h"
#include "administratordialog/rooms/administratorroomseditwidget.h"
#include "dialogs/messagetexteditor.h"
#include "dialogs/roomavatarwidget.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorRoomsEditWidgetTest)

AdministratorRoomsEditWidgetTest::AdministratorRoomsEditWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorRoomsEditWidgetTest::shouldHaveDefaultValues()
{
    AdministratorRoomsEditWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mLineEdit"));
    QVERIFY(mLineEdit);
    QVERIFY(mLineEdit->text().isEmpty());

    auto mOwnerName = w.findChild<QLabel *>(QStringLiteral("mOwnerName"));
    QVERIFY(mOwnerName);
    QVERIFY(mOwnerName->text().isEmpty());

    auto mDefaultCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mDefaultCheckBox"));
    QVERIFY(mDefaultCheckBox);
    QVERIFY(!mDefaultCheckBox->isChecked());
    QVERIFY(!mDefaultCheckBox->text().isEmpty());

    auto mFavoriteCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mFavoriteCheckBox"));
    QVERIFY(mFavoriteCheckBox);
    QVERIFY(!mFavoriteCheckBox->isChecked());
    QVERIFY(!mFavoriteCheckBox->text().isEmpty());

    auto mFeaturedCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mFeaturedCheckBox"));
    QVERIFY(mFeaturedCheckBox);
    QVERIFY(!mFeaturedCheckBox->isChecked());
    QVERIFY(!mFeaturedCheckBox->text().isEmpty());

    auto mPrivateCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mPrivateCheckBox"));
    QVERIFY(mPrivateCheckBox);
    QVERIFY(!mPrivateCheckBox->isChecked());
    QVERIFY(!mPrivateCheckBox->text().isEmpty());

    auto mReadOnlyCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mReadOnlyCheckBox"));
    QVERIFY(mReadOnlyCheckBox);
    QVERIFY(!mReadOnlyCheckBox->isChecked());
    QVERIFY(!mReadOnlyCheckBox->text().isEmpty());

    auto mArchivedCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mArchivedCheckBox"));
    QVERIFY(mArchivedCheckBox);
    QVERIFY(!mArchivedCheckBox->isChecked());
    QVERIFY(!mArchivedCheckBox->text().isEmpty());

    auto mComment = w.findChild<MessageTextEditor *>(QStringLiteral("mComment"));
    QVERIFY(mComment);

    auto mAnnouncement = w.findChild<MessageTextEditor *>(QStringLiteral("mAnnouncement"));
    QVERIFY(mAnnouncement);

    auto mDescription = w.findChild<MessageTextEditor *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);

    auto mRoomAvatarWidget = w.findChild<RoomAvatarWidget *>(QStringLiteral("mRoomAvatarWidget"));
    QVERIFY(mRoomAvatarWidget);
}

#include "moc_administratorroomseditwidgettest.cpp"
