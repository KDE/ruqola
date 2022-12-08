/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteuserswidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

AdministratorInviteUsersWidget::AdministratorInviteUsersWidget(QWidget *parent)
    : QWidget{parent}
    , mListEmails(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("formLayout"));

    auto label = new QLabel(i18n("Emails:"), this);
    label->setObjectName(QStringLiteral("label"));

    mListEmails->setObjectName(QStringLiteral("mListEmails"));
    mListEmails->setPlaceholderText(i18n("Use ',' for separate emails"));

    mainLayout->addWidget(label);
    mainLayout->addWidget(mListEmails);
    connect(mListEmails, &QLineEdit::textChanged, this, [this](const QString &text) {
        Q_EMIT updateButtonOk(!text.trimmed().isEmpty());
    });
}

AdministratorInviteUsersWidget::~AdministratorInviteUsersWidget() = default;

QStringList AdministratorInviteUsersWidget::emails() const
{
    return mListEmails->text().split(QLatin1Char(','));
}
