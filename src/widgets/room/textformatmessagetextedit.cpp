/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textformatmessagetextedit.h"

#include <QHBoxLayout>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
TextFormatMessageTextEdit::TextFormatMessageTextEdit(QWidget *parent)
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
}

TextFormatMessageTextEdit::~TextFormatMessageTextEdit() = default;
