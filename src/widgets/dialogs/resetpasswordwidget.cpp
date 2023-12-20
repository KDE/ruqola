/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpasswordwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KEmailAddress>
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

ResetPasswordWidget::ResetPasswordWidget(QWidget *parent)
    : QWidget{parent}
    , mEmail(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("Email:"), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mEmail->setObjectName(QStringLiteral("mEmail"));
    mainLayout->addWidget(mEmail);
    new LineEditCatchReturnKey(mEmail, this);
    connect(mEmail, &QLineEdit::textChanged, this, &ResetPasswordWidget::slotEmailChanged);
}

ResetPasswordWidget::~ResetPasswordWidget() = default;

void ResetPasswordWidget::slotEmailChanged(const QString &str)
{
    const QString email = str.trimmed();
    Q_EMIT updateOkButton(KEmailAddress::isValidAddress(email) == KEmailAddress::AddressOk);
}

QString ResetPasswordWidget::email() const
{
    return mEmail->text().trimmed();
}

#include "moc_resetpasswordwidget.cpp"
