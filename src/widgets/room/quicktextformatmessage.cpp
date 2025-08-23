/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "quicktextformatmessage.h"

#include <QHBoxLayout>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
QuickTextFormatMessage::QuickTextFormatMessage(QWidget *parent)
    : QFrame(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto boldButton = new QToolButton(this);
    boldButton->setObjectName(u"boldButton"_s);
    boldButton->setIconSize(QSize(12, 12));
    boldButton->setIcon(QIcon::fromTheme(u"format-text-bold"_s));
    mainLayout->addWidget(boldButton);

    auto italicButton = new QToolButton(this);
    italicButton->setObjectName(u"italicButton"_s);
    italicButton->setIconSize(QSize(12, 12));
    italicButton->setIcon(QIcon::fromTheme(u"format-text-italic"_s));
    mainLayout->addWidget(italicButton);

    auto strikeThroughButton = new QToolButton(this);
    strikeThroughButton->setObjectName(u"underlineButton"_s);
    strikeThroughButton->setIconSize(QSize(12, 12));
    strikeThroughButton->setIcon(QIcon::fromTheme(u"format-text-strikethrough"_s));
    mainLayout->addWidget(strikeThroughButton);

    auto codeBlockButton = new QToolButton(this);
    codeBlockButton->setObjectName(u"codeBlockButton"_s);
    codeBlockButton->setIconSize(QSize(12, 12));
    codeBlockButton->setIcon(QIcon::fromTheme(u"format-text-code"_s));
    mainLayout->addWidget(codeBlockButton);

    auto blockQuoteButton = new QToolButton(this);
    blockQuoteButton->setObjectName(u"blockQuoteButton"_s);
    blockQuoteButton->setIconSize(QSize(12, 12));
    blockQuoteButton->setIcon(QIcon::fromTheme(u"format-text-blockquote"_s));
    mainLayout->addWidget(blockQuoteButton);

    auto insertLinkButton = new QToolButton(this);
    insertLinkButton->setObjectName(u"insertLinkButton"_s);
    insertLinkButton->setIconSize(QSize(12, 12));
    insertLinkButton->setIcon(QIcon::fromTheme(u"link"_s));
    mainLayout->addWidget(insertLinkButton);
}

QuickTextFormatMessage::~QuickTextFormatMessage() = default;

#include "moc_quicktextformatmessage.cpp"
