/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "confirmpasswordwidget.h"

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <kwidgetsaddons_version.h>

ConfirmPasswordWidget::ConfirmPasswordWidget(QWidget *parent)
    : QWidget{parent}
    , mPasswordLineEdit(new KPasswordLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("For your security, you must enter your current password to continue"), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mainLayout->addWidget(mPasswordLineEdit);
#if KWIDGETSADDONS_VERSION < QT_VERSION_CHECK(5, 249, 0)
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
#else
    mPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPassword::RevealMode::OnlyNew
                                                                                                                : KPassword::RevealMode::Never);
#endif
    connect(mPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, [this](const QString &password) {
        Q_EMIT enabledOkButton(!password.isEmpty());
    });
}

ConfirmPasswordWidget::~ConfirmPasswordWidget() = default;

QString ConfirmPasswordWidget::password() const
{
    return mPasswordLineEdit->password();
}

#include "moc_confirmpasswordwidget.cpp"
