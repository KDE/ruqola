/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "myaccountpreferenceconfigurewidgettest.h"
#include "myaccount/myaccountpreferenceconfigurewidget.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccountPreferenceConfigureWidgetTest)
MyAccountPreferenceConfigureWidgetTest::MyAccountPreferenceConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MyAccountPreferenceConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountPreferenceConfigureWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto highlightWordsLabel = w.findChild<QLabel *>(QStringLiteral("highlightWordsLabel"));
    QVERIFY(highlightWordsLabel);
    QCOMPARE(highlightWordsLabel->textFormat(), Qt::PlainText);
    QVERIFY(!highlightWordsLabel->text().isEmpty());

    auto mHighlightWords = w.findChild<QLineEdit *>(QStringLiteral("mHighlightWords"));
    QVERIFY(mHighlightWords);
    QVERIFY(mHighlightWords->text().isEmpty());

    auto mDesktopNotification = w.findChild<QComboBox *>(QStringLiteral("mDesktopNotification"));
    QVERIFY(mDesktopNotification);
    auto mEmailNotification = w.findChild<QComboBox *>(QStringLiteral("mEmailNotification"));
    QVERIFY(mEmailNotification);
    auto mMobileNotification = w.findChild<QComboBox *>(QStringLiteral("mMobileNotification"));
    QVERIFY(mMobileNotification);

    auto desktopNotificationLabel = w.findChild<QLabel *>(QStringLiteral("desktopNotificationLabel"));
    QVERIFY(desktopNotificationLabel);
    QCOMPARE(desktopNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!desktopNotificationLabel->text().isEmpty());

    auto emailNotificationLabel = w.findChild<QLabel *>(QStringLiteral("emailNotificationLabel"));
    QVERIFY(emailNotificationLabel);
    QCOMPARE(emailNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!emailNotificationLabel->text().isEmpty());

    auto mobileNotificationLabel = w.findChild<QLabel *>(QStringLiteral("mobileNotificationLabel"));
    QVERIFY(mobileNotificationLabel);
    QCOMPARE(mobileNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!mobileNotificationLabel->text().isEmpty());

    auto mUseEmoji = w.findChild<QCheckBox *>(QStringLiteral("mUseEmoji"));
    QVERIFY(mUseEmoji);
    QVERIFY(!mUseEmoji->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mUseEmoji->text().isEmpty());

    auto mConvertAsciiEmoji = w.findChild<QCheckBox *>(QStringLiteral("mConvertAsciiEmoji"));
    QVERIFY(mConvertAsciiEmoji);
    QVERIFY(!mConvertAsciiEmoji->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mConvertAsciiEmoji->text().isEmpty());

    auto mHideRoles = w.findChild<QCheckBox *>(QStringLiteral("mHideRoles"));
    QVERIFY(mHideRoles);
    QVERIFY(!mHideRoles->isChecked());
    QVERIFY(!mHideRoles->text().isEmpty());
}
