/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomseditwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mLineEdit = w.findChild<QLineEdit *>(u"mLineEdit"_s);
    QVERIFY(mLineEdit);
    QVERIFY(mLineEdit->text().isEmpty());

    auto mOwnerName = w.findChild<QLabel *>(u"mOwnerName"_s);
    QVERIFY(mOwnerName);
    QVERIFY(mOwnerName->text().isEmpty());

    auto mDefaultCheckBox = w.findChild<QCheckBox *>(u"mDefaultCheckBox"_s);
    QVERIFY(mDefaultCheckBox);
    QVERIFY(!mDefaultCheckBox->isChecked());
    QVERIFY(!mDefaultCheckBox->text().isEmpty());

    auto mFavoriteCheckBox = w.findChild<QCheckBox *>(u"mFavoriteCheckBox"_s);
    QVERIFY(mFavoriteCheckBox);
    QVERIFY(!mFavoriteCheckBox->isChecked());
    QVERIFY(!mFavoriteCheckBox->text().isEmpty());

    auto mFeaturedCheckBox = w.findChild<QCheckBox *>(u"mFeaturedCheckBox"_s);
    QVERIFY(mFeaturedCheckBox);
    QVERIFY(!mFeaturedCheckBox->isChecked());
    QVERIFY(!mFeaturedCheckBox->text().isEmpty());

    auto mPrivateCheckBox = w.findChild<QCheckBox *>(u"mPrivateCheckBox"_s);
    QVERIFY(mPrivateCheckBox);
    QVERIFY(!mPrivateCheckBox->isChecked());
    QVERIFY(!mPrivateCheckBox->text().isEmpty());

    auto mReadOnlyCheckBox = w.findChild<QCheckBox *>(u"mReadOnlyCheckBox"_s);
    QVERIFY(mReadOnlyCheckBox);
    QVERIFY(!mReadOnlyCheckBox->isChecked());
    QVERIFY(!mReadOnlyCheckBox->text().isEmpty());

    auto mArchivedCheckBox = w.findChild<QCheckBox *>(u"mArchivedCheckBox"_s);
    QVERIFY(mArchivedCheckBox);
    QVERIFY(!mArchivedCheckBox->isChecked());
    QVERIFY(!mArchivedCheckBox->text().isEmpty());

    auto mComment = w.findChild<MessageTextEditor *>(u"mComment"_s);
    QVERIFY(mComment);

    auto mAnnouncement = w.findChild<MessageTextEditor *>(u"mAnnouncement"_s);
    QVERIFY(mAnnouncement);

    auto mDescription = w.findChild<MessageTextEditor *>(u"mDescription"_s);
    QVERIFY(mDescription);

    auto mRoomAvatarWidget = w.findChild<RoomAvatarWidget *>(u"mRoomAvatarWidget"_s);
    QVERIFY(mRoomAvatarWidget);
}

#include "moc_administratorroomseditwidgettest.cpp"
