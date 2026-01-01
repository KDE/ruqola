/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "confirmpasswordwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QLabel>
#include <QVBoxLayout>

ConfirmPasswordWidget::ConfirmPasswordWidget(QWidget *parent)
    : QWidget{parent}
    , mPasswordLineEdit(new KPasswordLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18nc("@label:textbox", "For your security, you must enter your current password to continue"), this);
    label->setObjectName(u"label"_s);
    mainLayout->addWidget(label);

    mPasswordLineEdit->setObjectName(u"mPasswordLineEdit"_s);
    mainLayout->addWidget(mPasswordLineEdit);
    mPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(u"lineedit_reveal_password"_s) ? KPassword::RevealMode::OnlyNew
                                                                                                   : KPassword::RevealMode::Never);
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
