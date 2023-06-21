/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesscreatewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

MyAccountPersonalAccessCreateWidget::MyAccountPersonalAccessCreateWidget(QWidget *parent)
    : QWidget{parent}
    , mBypassTwoFactor(new QCheckBox(i18n("Ignore Two Factor Authentication"), this))
    , mTokenName(new QLineEdit(this))
{
    auto formLayout = new QFormLayout(this);
    formLayout->setObjectName(QStringLiteral("formLayout"));

    mBypassTwoFactor->setObjectName(QStringLiteral("mBypassTwoFactor"));
    mTokenName->setObjectName(QStringLiteral("mTokenName"));
    mTokenName->setClearButtonEnabled(true);

    new LineEditCatchReturnKey(mTokenName, this);

    formLayout->addRow(i18n("Name:"), mTokenName);
    formLayout->addWidget(mBypassTwoFactor);

    connect(mTokenName, &QLineEdit::textChanged, this, [this](const QString &str) {
        Q_EMIT updateOkButton(!str.trimmed().isEmpty());
    });
}

MyAccountPersonalAccessCreateWidget::~MyAccountPersonalAccessCreateWidget() = default;

bool MyAccountPersonalAccessCreateWidget::bypassTwoFactor() const
{
    return mBypassTwoFactor->isChecked();
}

QString MyAccountPersonalAccessCreateWidget::tokenName() const
{
    return mTokenName->text().trimmed();
}

#include "moc_myaccountpersonalaccesscreatewidget.cpp"
