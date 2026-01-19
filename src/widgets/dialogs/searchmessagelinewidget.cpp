/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagelinewidget.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLineEdit>

using namespace Qt::Literals::StringLiterals;
SearchMessageLineWidget::SearchMessageLineWidget(QWidget *parent)
    : QWidget{parent}
    , mMainLayout(new QHBoxLayout(this))
    , mSearchMessageLineEdit(new QLineEdit(this))
{
    mMainLayout->setObjectName(u"mMainLayout"_s);
    mMainLayout->setContentsMargins({});
    mSearchMessageLineEdit->setObjectName(u"mSearchMessageLineEdit"_s);
    mMainLayout->addWidget(mSearchMessageLineEdit);
    mSearchMessageLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchMessageLineEdit);
    mSearchMessageLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search messages…"));
    connect(mSearchMessageLineEdit, &QLineEdit::textChanged, this, &SearchMessageLineWidget::textChanged);
}

SearchMessageLineWidget::~SearchMessageLineWidget() = default;

void SearchMessageLineWidget::addMessageLineWidget(QWidget *w)
{
    mMainLayout->addWidget(w);
}

#include "moc_searchmessagelinewidget.cpp"
