/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchteamwidget.h"
#include "searchteamcompletionlineedit.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>

SearchTeamWidget::SearchTeamWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchLine(new SearchTeamCompletionLineEdit(account, this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto label = new QLabel(i18nc("@label:textbox", "Team Name:"), this);
    label->setObjectName(QStringLiteral("label"));
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mSearchLine->setObjectName(QStringLiteral("mSearchLine"));
    mainLayout->addWidget(mSearchLine);
    mainLayout->addStretch(1);
    connect(mSearchLine, &SearchTeamCompletionLineEdit::teamIdChanged, this, [this](bool isEmpty) {
        Q_EMIT updateOkButton(!isEmpty);
    });
}

SearchTeamWidget::~SearchTeamWidget() = default;

const QByteArray &SearchTeamWidget::teamId() const
{
    return mSearchLine->teamId();
}

#include "moc_searchteamwidget.cpp"
