/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emailsettingswidgettest.h"
#include "administratorsettingsdialog/email/emailsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QTest>
QTEST_MAIN(EmailSettingsWidgetTest)
EmailSettingsWidgetTest::EmailSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EmailSettingsWidgetTest::shouldHaveDefaultValues()
{
    EmailSettingsWidget w(nullptr);
    auto smtpLabel = w.findChild<QLabel *>(QStringLiteral("smtpLabel"));
    QVERIFY(smtpLabel);
    QVERIFY(!smtpLabel->text().isEmpty());

    auto mSmtpHost = w.findChild<QLineEdit *>(QStringLiteral("mSmtpHost"));
    QVERIFY(mSmtpHost);
    QVERIFY(mSmtpHost->text().isEmpty());
    QVERIFY(mSmtpHost->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpHost), QStringLiteral("SMTP_Host"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SMTP_Host"));

    auto mSmtpPort = w.findChild<QSpinBox *>(QStringLiteral("mSmtpPort"));
    QVERIFY(mSmtpPort);
    QVERIFY(mSmtpPort->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpPort), QStringLiteral("SMTP_Port"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SMTP_Port"));

    auto mIgnoreTls = w.findChild<QCheckBox *>(QStringLiteral("mIgnoreTls"));
    QVERIFY(mIgnoreTls);
    QVERIFY(!mIgnoreTls->isChecked());
    QVERIFY(!mIgnoreTls->text().isEmpty());
    QVERIFY(mIgnoreTls->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mIgnoreTls), QStringLiteral("SMTP_IgnoreTLS"));

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QVERIFY(mUserName->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUserName), QStringLiteral("SMTP_Username"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SMTP_Username"));

    auto mFromEmail = w.findChild<QLineEdit *>(QStringLiteral("mFromEmail"));
    QVERIFY(mFromEmail);
    QVERIFY(mFromEmail->text().isEmpty());
    QVERIFY(mFromEmail->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFromEmail), QStringLiteral("From_Email"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("From_Email"));

    auto privacyLabel = w.findChild<QLabel *>(QStringLiteral("privacyLabel"));
    QVERIFY(privacyLabel);
    QVERIFY(!privacyLabel->text().isEmpty());

    auto mShowMessageEmailNotification = w.findChild<QCheckBox *>(QStringLiteral("mShowMessageEmailNotification"));
    QVERIFY(mShowMessageEmailNotification);
    QVERIFY(!mShowMessageEmailNotification->isChecked());
    QVERIFY(!mShowMessageEmailNotification->text().isEmpty());
    QVERIFY(mShowMessageEmailNotification->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mShowMessageEmailNotification), QStringLiteral("Email_notification_show_message"));

    auto mAddSenderReplyTo = w.findChild<QCheckBox *>(QStringLiteral("mAddSenderReplyTo"));
    QVERIFY(mAddSenderReplyTo);
    QVERIFY(!mAddSenderReplyTo->isChecked());
    QVERIFY(!mAddSenderReplyTo->text().isEmpty());
    QVERIFY(mAddSenderReplyTo->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAddSenderReplyTo), QStringLiteral("Add_Sender_To_ReplyTo"));

    auto directReplyLabel = w.findChild<QLabel *>(QStringLiteral("directReplyLabel"));
    QVERIFY(directReplyLabel);
    QVERIFY(!directReplyLabel->text().isEmpty());

    auto mEnableDirectReply = w.findChild<QCheckBox *>(QStringLiteral("mEnableDirectReply"));
    QVERIFY(mEnableDirectReply);
    QVERIFY(!mEnableDirectReply->isChecked());
    QVERIFY(!mEnableDirectReply->text().isEmpty());
    QVERIFY(!mEnableDirectReply->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableDirectReply), QStringLiteral("Direct_Reply_Enable"));

    auto mDebugDirectReply = w.findChild<QCheckBox *>(QStringLiteral("mDebugDirectReply"));
    QVERIFY(mDebugDirectReply);
    QVERIFY(!mDebugDirectReply->isChecked());
    QVERIFY(!mDebugDirectReply->text().isEmpty());
    QVERIFY(!mDebugDirectReply->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDebugDirectReply), QStringLiteral("Direct_Reply_Debug"));

    auto forgotPasswordLabel = w.findChild<QLabel *>(QStringLiteral("forgotPasswordLabel"));
    QVERIFY(forgotPasswordLabel);
    QVERIFY(!forgotPasswordLabel->text().isEmpty());

    auto subjectLabel = w.findChild<QLabel *>(QStringLiteral("subjectLabel"));
    QVERIFY(subjectLabel);
    QVERIFY(!subjectLabel->text().isEmpty());

    auto mDirectMessageEmailSubject = w.findChild<QPlainTextEdit *>(QStringLiteral("mDirectMessageEmailSubject"));
    QVERIFY(mDirectMessageEmailSubject);
    QVERIFY(mDirectMessageEmailSubject->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDirectMessageEmailSubject), QStringLiteral("Offline_DM_Email"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Offline_DM_Email"));

    auto mMentionEmailSubject = w.findChild<QPlainTextEdit *>(QStringLiteral("mMentionEmailSubject"));
    QVERIFY(mMentionEmailSubject);
    QVERIFY(mMentionEmailSubject->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMentionEmailSubject), QStringLiteral("Offline_Mention_Email"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Offline_Mention_Email"));

    auto mMentionAllEmailSubject = w.findChild<QPlainTextEdit *>(QStringLiteral("mMentionAllEmailSubject"));
    QVERIFY(mMentionAllEmailSubject);
    QVERIFY(mMentionAllEmailSubject->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMentionAllEmailSubject), QStringLiteral("Offline_Mention_All_Email"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Offline_Mention_All_Email"));

    auto verificationLabel = w.findChild<QLabel *>(QStringLiteral("verificationLabel"));
    QVERIFY(verificationLabel);
    QVERIFY(!verificationLabel->text().isEmpty());

    auto mVerificationSubject = w.findChild<QLineEdit *>(QStringLiteral("mVerificationSubject"));
    QVERIFY(mVerificationSubject);
    QVERIFY(mVerificationSubject->text().isEmpty());
    QVERIFY(mVerificationSubject->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mVerificationSubject), QStringLiteral("Verification_Email_Subject"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Verification_Email_Subject"));

    auto mVerificationBody = w.findChild<QPlainTextEdit *>(QStringLiteral("mVerificationBody"));
    QVERIFY(mVerificationBody);
    QVERIFY(mVerificationBody->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mVerificationBody), QStringLiteral("Verification_Email"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Verification_Email"));

    auto mForgotPasswordSubject = w.findChild<QLineEdit *>(QStringLiteral("mForgotPasswordSubject"));
    QVERIFY(mForgotPasswordSubject);
    QVERIFY(mForgotPasswordSubject->text().isEmpty());
    QVERIFY(mForgotPasswordSubject->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForgotPasswordSubject), QStringLiteral("Forgot_Password_Email_Subject"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Forgot_Password_Email_Subject"));

    auto mForgotPasswordBody = w.findChild<QPlainTextEdit *>(QStringLiteral("mForgotPasswordBody"));
    QVERIFY(mForgotPasswordBody);
    QVERIFY(mForgotPasswordBody->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForgotPasswordBody), QStringLiteral("Forgot_Password_Email"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Forgot_Password_Email"));

    auto emailAddressChangedLabel = w.findChild<QLabel *>(QStringLiteral("emailAddressChangedLabel"));
    QVERIFY(emailAddressChangedLabel);
    QVERIFY(!emailAddressChangedLabel->text().isEmpty());

    auto mEmailAddressChangedSubject = w.findChild<QLineEdit *>(QStringLiteral("mEmailAddressChangedSubject"));
    QVERIFY(mEmailAddressChangedSubject);
    QVERIFY(mEmailAddressChangedSubject->text().isEmpty());
    QVERIFY(mEmailAddressChangedSubject->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEmailAddressChangedSubject), QStringLiteral("Email_Changed_Email_Subject"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Email_Changed_Email_Subject"));

    auto mEmailAddressChangedBody = w.findChild<QPlainTextEdit *>(QStringLiteral("mEmailAddressChangedBody"));
    QVERIFY(mEmailAddressChangedBody);
    QVERIFY(mEmailAddressChangedBody->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEmailAddressChangedBody), QStringLiteral("Email_Changed_Email"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Email_Changed_Email"));

    auto invitationLabel = w.findChild<QLabel *>(QStringLiteral("invitationLabel"));
    QVERIFY(invitationLabel);
    QVERIFY(!invitationLabel->text().isEmpty());

    auto mInvitationSubject = w.findChild<QLineEdit *>(QStringLiteral("mInvitationSubject"));
    QVERIFY(mInvitationSubject);
    QVERIFY(mInvitationSubject->text().isEmpty());
    QVERIFY(mInvitationSubject->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mInvitationSubject), QStringLiteral("Invitation_Subject"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Invitation_Subject"));

    auto mInvitationBody = w.findChild<QPlainTextEdit *>(QStringLiteral("mInvitationBody"));
    QVERIFY(mInvitationBody);
    QVERIFY(mInvitationBody->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mInvitationBody), QStringLiteral("Invitation_Email"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Invitation_Email"));

    auto mSmtpProtocol = w.findChild<QComboBox *>(QStringLiteral("mSmtpProtocol"));
    QVERIFY(mSmtpProtocol);
    QCOMPARE(mSmtpProtocol->count(), 2);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpProtocol), QStringLiteral("SMTP_Protocol"));
}
