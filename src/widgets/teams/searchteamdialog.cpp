/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchteamdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "searchteamwidget.h"
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

SearchTeamDialog::SearchTeamDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mSearchTeamWidget(new SearchTeamWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Search Team"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mSearchTeamWidget->setObjectName(u"mSearchTeamWidget"_s);
    mainLayout->addWidget(mSearchTeamWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &SearchTeamDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &SearchTeamDialog::accept);
    auto okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false); // Disabled by default
    connect(mSearchTeamWidget, &SearchTeamWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

SearchTeamDialog::~SearchTeamDialog() = default;

const QByteArray &SearchTeamDialog::teamId() const
{
    return mSearchTeamWidget->teamId();
}

#include "moc_searchteamdialog.cpp"
