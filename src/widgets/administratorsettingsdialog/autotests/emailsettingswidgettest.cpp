/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emailsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/email/emailsettingswidget.h"
#include "settingswidgetshelper.h"
#include <KPasswordLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpinBox>
QTEST_MAIN(EmailSettingsWidgetTest)
EmailSettingsWidgetTest::EmailSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EmailSettingsWidgetTest::shouldHaveDefaultValues()
{
    EmailSettingsWidget w(nullptr);
    auto smtpLabel = w.findChild<QLabel *>(u"smtpLabel"_s);
    QVERIFY(smtpLabel);
    QVERIFY(!smtpLabel->text().isEmpty());

    auto mSmtpHost = w.findChild<QLineEdit *>(u"mSmtpHost"_s);
    QVERIFY(mSmtpHost);
    QVERIFY(mSmtpHost->text().isEmpty());
    QVERIFY(mSmtpHost->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpHost), u"SMTP_Host"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"SMTP_Host"_s);

    auto mSmtpPort = w.findChild<QSpinBox *>(u"mSmtpPort"_s);
    QVERIFY(mSmtpPort);
    QVERIFY(mSmtpPort->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpPort), u"SMTP_Port"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"SMTP_Port"_s);

    auto mIgnoreTls = w.findChild<QCheckBox *>(u"mIgnoreTls"_s);
    QVERIFY(mIgnoreTls);
    QVERIFY(!mIgnoreTls->isChecked());
    QVERIFY(!mIgnoreTls->text().isEmpty());
    QVERIFY(mIgnoreTls->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mIgnoreTls), u"SMTP_IgnoreTLS"_s);

    auto mUserName = w.findChild<QLineEdit *>(u"mUserName"_s);
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QVERIFY(mUserName->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUserName), u"SMTP_Username"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"SMTP_Username"_s);

    auto mFromEmail = w.findChild<QLineEdit *>(u"mFromEmail"_s);
    QVERIFY(mFromEmail);
    QVERIFY(mFromEmail->text().isEmpty());
    QVERIFY(mFromEmail->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFromEmail), u"From_Email"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"From_Email"_s);

    auto mSmtpPassword = w.findChild<KPasswordLineEdit *>(u"mSmtpPassword"_s);
    QVERIFY(mSmtpPassword);
    QVERIFY(mSmtpPassword->password().isEmpty());
    QVERIFY(mSmtpPassword->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpPassword), u"SMTP_Password"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"SMTP_Password"_s);

    auto privacyLabel = w.findChild<QLabel *>(u"privacyLabel"_s);
    QVERIFY(privacyLabel);
    QVERIFY(!privacyLabel->text().isEmpty());

    auto mShowMessageEmailNotification = w.findChild<QCheckBox *>(u"mShowMessageEmailNotification"_s);
    QVERIFY(mShowMessageEmailNotification);
    QVERIFY(!mShowMessageEmailNotification->isChecked());
    QVERIFY(!mShowMessageEmailNotification->text().isEmpty());
    QVERIFY(mShowMessageEmailNotification->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mShowMessageEmailNotification), u"Email_notification_show_message"_s);

    auto mAddSenderReplyTo = w.findChild<QCheckBox *>(u"mAddSenderReplyTo"_s);
    QVERIFY(mAddSenderReplyTo);
    QVERIFY(!mAddSenderReplyTo->isChecked());
    QVERIFY(!mAddSenderReplyTo->text().isEmpty());
    QVERIFY(mAddSenderReplyTo->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAddSenderReplyTo), u"Add_Sender_To_ReplyTo"_s);

    auto directReplyLabel = w.findChild<QLabel *>(u"directReplyLabel"_s);
    QVERIFY(directReplyLabel);
    QVERIFY(!directReplyLabel->text().isEmpty());

    auto mEnableDirectReply = w.findChild<QCheckBox *>(u"mEnableDirectReply"_s);
    QVERIFY(mEnableDirectReply);
    QVERIFY(!mEnableDirectReply->isChecked());
    QVERIFY(!mEnableDirectReply->text().isEmpty());
    QVERIFY(!mEnableDirectReply->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableDirectReply), u"Direct_Reply_Enable"_s);

    auto mDebugDirectReply = w.findChild<QCheckBox *>(u"mDebugDirectReply"_s);
    QVERIFY(mDebugDirectReply);
    QVERIFY(!mDebugDirectReply->isChecked());
    QVERIFY(!mDebugDirectReply->text().isEmpty());
    QVERIFY(!mDebugDirectReply->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDebugDirectReply), u"Direct_Reply_Debug"_s);

    auto forgotPasswordLabel = w.findChild<QLabel *>(u"forgotPasswordLabel"_s);
    QVERIFY(forgotPasswordLabel);
    QVERIFY(!forgotPasswordLabel->text().isEmpty());

    auto subjectLabel = w.findChild<QLabel *>(u"subjectLabel"_s);
    QVERIFY(subjectLabel);
    QVERIFY(!subjectLabel->text().isEmpty());

    auto mDirectMessageEmailSubject = w.findChild<QPlainTextEdit *>(u"mDirectMessageEmailSubject"_s);
    QVERIFY(mDirectMessageEmailSubject);
    QVERIFY(mDirectMessageEmailSubject->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDirectMessageEmailSubject), u"Offline_DM_Email"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Offline_DM_Email"_s);

    auto mMentionEmailSubject = w.findChild<QPlainTextEdit *>(u"mMentionEmailSubject"_s);
    QVERIFY(mMentionEmailSubject);
    QVERIFY(mMentionEmailSubject->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMentionEmailSubject), u"Offline_Mention_Email"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Offline_Mention_Email"_s);

    auto mMentionAllEmailSubject = w.findChild<QPlainTextEdit *>(u"mMentionAllEmailSubject"_s);
    QVERIFY(mMentionAllEmailSubject);
    QVERIFY(mMentionAllEmailSubject->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMentionAllEmailSubject), u"Offline_Mention_All_Email"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Offline_Mention_All_Email"_s);

    auto verificationLabel = w.findChild<QLabel *>(u"verificationLabel"_s);
    QVERIFY(verificationLabel);
    QVERIFY(!verificationLabel->text().isEmpty());

    auto mVerificationSubject = w.findChild<QLineEdit *>(u"mVerificationSubject"_s);
    QVERIFY(mVerificationSubject);
    QVERIFY(mVerificationSubject->text().isEmpty());
    QVERIFY(mVerificationSubject->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mVerificationSubject), u"Verification_Email_Subject"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Verification_Email_Subject"_s);

    auto mVerificationBody = w.findChild<QPlainTextEdit *>(u"mVerificationBody"_s);
    QVERIFY(mVerificationBody);
    QVERIFY(mVerificationBody->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mVerificationBody), u"Verification_Email"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Verification_Email"_s);

    auto mForgotPasswordSubject = w.findChild<QLineEdit *>(u"mForgotPasswordSubject"_s);
    QVERIFY(mForgotPasswordSubject);
    QVERIFY(mForgotPasswordSubject->text().isEmpty());
    QVERIFY(mForgotPasswordSubject->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForgotPasswordSubject), u"Forgot_Password_Email_Subject"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Forgot_Password_Email_Subject"_s);

    auto mForgotPasswordBody = w.findChild<QPlainTextEdit *>(u"mForgotPasswordBody"_s);
    QVERIFY(mForgotPasswordBody);
    QVERIFY(mForgotPasswordBody->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForgotPasswordBody), u"Forgot_Password_Email"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Forgot_Password_Email"_s);

    auto emailAddressChangedLabel = w.findChild<QLabel *>(u"emailAddressChangedLabel"_s);
    QVERIFY(emailAddressChangedLabel);
    QVERIFY(!emailAddressChangedLabel->text().isEmpty());

    auto mEmailAddressChangedSubject = w.findChild<QLineEdit *>(u"mEmailAddressChangedSubject"_s);
    QVERIFY(mEmailAddressChangedSubject);
    QVERIFY(mEmailAddressChangedSubject->text().isEmpty());
    QVERIFY(mEmailAddressChangedSubject->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEmailAddressChangedSubject), u"Email_Changed_Email_Subject"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Email_Changed_Email_Subject"_s);

    auto mEmailAddressChangedBody = w.findChild<QPlainTextEdit *>(u"mEmailAddressChangedBody"_s);
    QVERIFY(mEmailAddressChangedBody);
    QVERIFY(mEmailAddressChangedBody->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEmailAddressChangedBody), u"Email_Changed_Email"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Email_Changed_Email"_s);

    auto invitationLabel = w.findChild<QLabel *>(u"invitationLabel"_s);
    QVERIFY(invitationLabel);
    QVERIFY(!invitationLabel->text().isEmpty());

    auto mInvitationSubject = w.findChild<QLineEdit *>(u"mInvitationSubject"_s);
    QVERIFY(mInvitationSubject);
    QVERIFY(mInvitationSubject->text().isEmpty());
    QVERIFY(mInvitationSubject->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mInvitationSubject), u"Invitation_Subject"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Invitation_Subject"_s);

    auto mInvitationBody = w.findChild<QPlainTextEdit *>(u"mInvitationBody"_s);
    QVERIFY(mInvitationBody);
    QVERIFY(mInvitationBody->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mInvitationBody), u"Invitation_Email"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Invitation_Email"_s);

    auto mSmtpProtocol = w.findChild<QComboBox *>(u"mSmtpProtocol"_s);
    QVERIFY(mSmtpProtocol);
    QCOMPARE(mSmtpProtocol->count(), 2);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpProtocol), u"SMTP_Protocol"_s);

    auto mPasswordChangedSubject = w.findChild<QLineEdit *>(u"mPasswordChangedSubject"_s);
    QVERIFY(mPasswordChangedSubject);
    QVERIFY(mPasswordChangedSubject->text().isEmpty());
    QVERIFY(mPasswordChangedSubject->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPasswordChangedSubject), u"Password_Changed_Email_Subject"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Password_Changed_Email_Subject"_s);

    auto mPasswordChangedBody = w.findChild<QPlainTextEdit *>(u"mPasswordChangedBody"_s);
    QVERIFY(mPasswordChangedBody);
    QVERIFY(mPasswordChangedBody->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPasswordChangedBody), u"Password_Changed_Email"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Password_Changed_Email"_s);
}

#include "moc_emailsettingswidgettest.cpp"
