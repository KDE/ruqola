/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchwithdelaylineedit.h"
#include <KLocalizedString>
#include <QTimer>

SearchWithDelayLineEdit::SearchWithDelayLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , mSearchTimer(new QTimer(this))
{
    setClearButtonEnabled(true);
    setPlaceholderText(i18n("Search Word..."));
    connect(mSearchTimer, &QTimer::timeout, this, &SearchWithDelayLineEdit::slotSearchTimerFired);
    connect(this, &SearchWithDelayLineEdit::textChanged, this, &SearchWithDelayLineEdit::slotSearchTextEdited);
    connect(this, &SearchWithDelayLineEdit::returnPressed, this, &SearchWithDelayLineEdit::slotSearchTimerFired);
}

SearchWithDelayLineEdit::~SearchWithDelayLineEdit() = default;

void SearchWithDelayLineEdit::setDelayMs(std::chrono::milliseconds value)
{
    mMilliseconds = value;
}

void SearchWithDelayLineEdit::slotSearchTimerFired()
{
    mSearchTimer->stop();
    if (!text().trimmed().isEmpty()) {
        Q_EMIT searchRequested(text());
    } else {
        Q_EMIT searchCleared();
    }
}

void SearchWithDelayLineEdit::slotSearchTextEdited()
{
    if (mSearchTimer->isActive()) {
        mSearchTimer->stop(); // eventually
    }

    mSearchTimer->setSingleShot(true);
    mSearchTimer->start(mMilliseconds);
}

#include "moc_searchwithdelaylineedit.cpp"
