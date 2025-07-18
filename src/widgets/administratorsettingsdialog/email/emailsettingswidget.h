/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QComboBox;
class QLineEdit;
class QSpinBox;
class QCheckBox;
class QPlainTextEdit;
class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT EmailSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit EmailSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~EmailSettingsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QComboBox *const mSmtpProtocol;
    QLineEdit *const mSmtpHost;
    QSpinBox *const mSmtpPort;
    QCheckBox *const mSmtpIgnoreTls;
    QLineEdit *const mSmtpUserName;
    KPasswordLineEdit *const mSmtpPassword;
    QLineEdit *const mSmtpFromEmail;
    QCheckBox *const mShowMessageEmailNotification;
    QCheckBox *const mAddSenderReplyTo;
    QCheckBox *const mEnableDirectReply;
    QCheckBox *const mDebugDirectReply;
    QPlainTextEdit *const mDirectMessageEmailSubject;
    QPlainTextEdit *const mMentionEmailSubject;
    QPlainTextEdit *const mMentionAllEmailSubject;
    QLineEdit *const mVerificationSubject;
    QPlainTextEdit *const mVerificationBody;

    QLineEdit *const mForgotPasswordSubject;
    QPlainTextEdit *const mForgotPasswordBody;

    QLineEdit *const mEmailAddressChangedSubject;
    QPlainTextEdit *const mEmailAddressChangedBody;

    QLineEdit *const mInvitationSubject;
    QPlainTextEdit *const mInvitationBody;

    QLineEdit *const mPasswordChangedSubject;
    QPlainTextEdit *const mPasswordChangedBody;
};
