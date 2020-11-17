/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "myaccountpreferenceconfigurewidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLocalizedString>
#include <QLineEdit>

MyAccountPreferenceConfigureWidget::MyAccountPreferenceConfigureWidget(QWidget *parent)
    : QWidget(parent)
    , mHighlightWords(new QLineEdit(this))
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto *highlightWordsLabel = new QLabel(i18n("Highlight words:"), this);
    highlightWordsLabel->setObjectName(QStringLiteral("highlightWordsLabel"));
    highlightWordsLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(highlightWordsLabel);

    mHighlightWords->setObjectName(QStringLiteral("mHighlightWords"));
    mHighlightWords->setPlaceholderText(i18n("Use \',\' for separating words"));
    mainLayout->addWidget(mHighlightWords);
    mainLayout->addStretch();
}

MyAccountPreferenceConfigureWidget::~MyAccountPreferenceConfigureWidget()
{
}

void MyAccountPreferenceConfigureWidget::save()
{
}

void MyAccountPreferenceConfigureWidget::load()
{
    mHighlightWords->setText(Ruqola::self()->rocketChatAccount()->highlightWords().join(QLatin1Char(',')));
}
