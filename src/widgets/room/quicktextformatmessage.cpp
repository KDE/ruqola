/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "quicktextformatmessage.h"

#include <QHBoxLayout>
#include <QTextEdit>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
QuickTextFormatMessage::QuickTextFormatMessage(QTextEdit *editor, QWidget *parent)
    : QFrame(parent)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto boldButton = new QToolButton(this);
    boldButton->setObjectName(u"boldButton"_s);
    boldButton->setIconSize(QSize(12, 12));
    boldButton->setIcon(QIcon::fromTheme(u"format-text-bold"_s));
    mainLayout->addWidget(boldButton);
    connect(boldButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::Bold);
    });

    auto italicButton = new QToolButton(this);
    italicButton->setObjectName(u"italicButton"_s);
    italicButton->setIconSize(QSize(12, 12));
    italicButton->setIcon(QIcon::fromTheme(u"format-text-italic"_s));
    connect(italicButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::Italic);
    });
    mainLayout->addWidget(italicButton);

    auto strikeThroughButton = new QToolButton(this);
    strikeThroughButton->setObjectName(u"strikeThroughButton"_s);
    strikeThroughButton->setIconSize(QSize(12, 12));
    strikeThroughButton->setIcon(QIcon::fromTheme(u"format-text-strikethrough"_s));
    connect(strikeThroughButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::StrikeThrough);
    });
    mainLayout->addWidget(strikeThroughButton);

    auto codeBlockButton = new QToolButton(this);
    codeBlockButton->setObjectName(u"codeBlockButton"_s);
    codeBlockButton->setIconSize(QSize(12, 12));
    codeBlockButton->setIcon(QIcon::fromTheme(u"format-text-code"_s));
    connect(codeBlockButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::CodeBlock);
    });

    mainLayout->addWidget(codeBlockButton);

    auto blockQuoteButton = new QToolButton(this);
    blockQuoteButton->setObjectName(u"blockQuoteButton"_s);
    blockQuoteButton->setIconSize(QSize(12, 12));
    blockQuoteButton->setIcon(QIcon::fromTheme(u"format-text-blockquote"_s));
    connect(blockQuoteButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::BlockQuote);
    });

    mainLayout->addWidget(blockQuoteButton);

    auto insertLinkButton = new QToolButton(this);
    insertLinkButton->setObjectName(u"insertLinkButton"_s);
    insertLinkButton->setIconSize(QSize(12, 12));
    insertLinkButton->setIcon(QIcon::fromTheme(u"link"_s));
    connect(insertLinkButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::InsertLink);
    });
    mainLayout->addWidget(insertLinkButton);

    if (editor) {
        connect(editor, &QTextEdit::selectionChanged, this, [this, editor]() {
            if (editor->textCursor().hasSelection()) {
                const QRect cursorRect = editor->cursorRect();
                const QPoint globalPos = editor->viewport()->mapToGlobal(cursorRect.topLeft());
                move(globalPos.x(), globalPos.y() - height());
                show();
            } else {
                hide();
            }
        });
    }
}

QuickTextFormatMessage::~QuickTextFormatMessage() = default;

#include "moc_quicktextformatmessage.cpp"
