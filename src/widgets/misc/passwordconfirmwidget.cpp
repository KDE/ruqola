/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordconfirmwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/passwordvalidatewidget.h"

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <KStatefulBrush>
#include <QFormLayout>
#include <QLabel>

PasswordConfirmWidget::PasswordConfirmWidget(QWidget *parent)
    : QWidget(parent)
    , mNewPasswordLineEdit(new KPasswordLineEdit(this))
    , mConfirmPasswordLineEdit(new KPasswordLineEdit(this))
    , mPasswordValidateWidget(new PasswordValidateWidget(this))
    , mInvalidPassword(new QLabel(i18n("Your passwords do no match."), this))
{
    const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeText);
    const QColor color = bgBrush.brush(palette()).color();
    QPalette pal = this->palette();
    pal.setColor(QPalette::WindowText, color);
    mInvalidPassword->setPalette(pal);

    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mNewPasswordLineEdit->setObjectName(u"mNewPasswordLineEdit"_s);

    mainLayout->addRow(i18n("New Password:"), mNewPasswordLineEdit);
    mNewPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(u"lineedit_reveal_password"_s) ? KPassword::RevealMode::OnlyNew
                                                                                                      : KPassword::RevealMode::Never);
    mPasswordValidateWidget->setObjectName(u"mPasswordValidateWidget"_s);
    mainLayout->addWidget(mPasswordValidateWidget);
    connect(mNewPasswordLineEdit, &KPasswordLineEdit::passwordChanged, mPasswordValidateWidget, &PasswordValidateWidget::validatePassword);
    connect(mPasswordValidateWidget, &PasswordValidateWidget::passwordIsValid, mConfirmPasswordLineEdit, &KPasswordLineEdit::setEnabled);

    mConfirmPasswordLineEdit->setObjectName(u"mConfirmPasswordLineEdit"_s);
    mainLayout->addRow(i18n("Confirm Password:"), mConfirmPasswordLineEdit);
    mConfirmPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(u"lineedit_reveal_password"_s) ? KPassword::RevealMode::OnlyNew
                                                                                                          : KPassword::RevealMode::Never);
    mInvalidPassword->setObjectName(u"mInvalidPassword"_s);
    mInvalidPassword->setVisible(false);
    mainLayout->addWidget(mInvalidPassword);
    connect(mConfirmPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &PasswordConfirmWidget::slotVerifyPassword);
    connect(mNewPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &PasswordConfirmWidget::slotVerifyPassword);
}

PasswordConfirmWidget::~PasswordConfirmWidget() = default;

void PasswordConfirmWidget::slotVerifyPassword()
{
    const bool state = isNewPasswordConfirmed();
    mInvalidPassword->setVisible(!state);
    Q_EMIT passwordValidated(state);
}

bool PasswordConfirmWidget::isNewPasswordConfirmed() const
{
    const QString password = mConfirmPasswordLineEdit->password();
    return !password.isEmpty() && (password == mNewPasswordLineEdit->password());
}

QString PasswordConfirmWidget::password() const
{
    return mConfirmPasswordLineEdit->password();
}

void PasswordConfirmWidget::setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings)
{
    mPasswordValidateWidget->setPasswordValidChecks(passwordSettings);
}

#include "moc_passwordconfirmwidget.cpp"
