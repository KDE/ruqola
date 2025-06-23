/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesscreatewidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

MyAccountPersonalAccessCreateWidget::MyAccountPersonalAccessCreateWidget(QWidget *parent)
    : QWidget{parent}
    , mBypassTwoFactor(new QCheckBox(i18nc("@option:check", "Ignore Two Factor Authentication"), this))
    , mTokenName(new QLineEdit(this))
{
    auto formLayout = new QFormLayout(this);
    formLayout->setObjectName(u"formLayout"_s);

    mBypassTwoFactor->setObjectName(u"mBypassTwoFactor"_s);
    mTokenName->setObjectName(u"mTokenName"_s);
    mTokenName->setClearButtonEnabled(true);

    KLineEditEventHandler::catchReturnKey(mTokenName);

    formLayout->addRow(i18n("Name:"), mTokenName);
    formLayout->addWidget(mBypassTwoFactor);

    connect(mTokenName, &QLineEdit::textChanged, this, [this](const QString &str) {
        const QString strTrimmed = str.trimmed();
        Q_EMIT updateOkButton(!strTrimmed.isEmpty() && !mExistingTokenNames.contains(strTrimmed));
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

void MyAccountPersonalAccessCreateWidget::setExistingTokenNames(const QStringList &lst)
{
    mExistingTokenNames = lst;
}

#include "moc_myaccountpersonalaccesscreatewidget.cpp"
