/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteuserswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"formLayout"_s);

    auto label = new QLabel(i18nc("@label:textbox", "Emails:"), this);
    label->setObjectName(u"label"_s);

    mListEmails->setObjectName(u"mListEmails"_s);
    mListEmails->setPlaceholderText(i18nc("@info:placeholder", "Use ',' for separate emails"));

    mainLayout->addWidget(label);
    mainLayout->addWidget(mListEmails);
    connect(mListEmails, &QLineEdit::textChanged, this, [this](const QString &text) {
        Q_EMIT updateButtonOk(!text.trimmed().isEmpty());
    });
}

AdministratorInviteUsersWidget::~AdministratorInviteUsersWidget() = default;

QStringList AdministratorInviteUsersWidget::emails() const
{
    return mListEmails->text().split(u',', Qt::SkipEmptyParts);
}

#include "moc_administratorinviteuserswidget.cpp"
