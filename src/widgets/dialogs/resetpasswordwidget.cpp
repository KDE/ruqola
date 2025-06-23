/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpasswordwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KEmailAddress>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

ResetPasswordWidget::ResetPasswordWidget(QWidget *parent)
    : QWidget{parent}
    , mEmail(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18nc("@label:textbox", "Email:"), this);
    label->setObjectName(u"label"_s);
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mEmail->setObjectName(u"mEmail"_s);
    mainLayout->addWidget(mEmail);
    KLineEditEventHandler::catchReturnKey(mEmail);
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
