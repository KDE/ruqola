/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "quicktextformatmessage.h"

#include <KLocalizedString>
#include <KSeparator>

#include <QHBoxLayout>
#include <QTextEdit>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
QuickTextFormatMessage::QuickTextFormatMessage(QTextEdit *editor, QWidget *parent)
    : QFrame(parent)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    setFocusPolicy(Qt::NoFocus);

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto boldButton = new QToolButton(this);
    boldButton->setFocusPolicy(Qt::NoFocus);
    boldButton->setObjectName(u"boldButton"_s);
    boldButton->setIconSize(QSize(12, 12));
    boldButton->setIcon(QIcon::fromTheme(u"format-text-bold"_s));
    boldButton->setAutoRaise(true);
    boldButton->setToolTip(i18nc("@info:tooltip", "Bold"));
    mainLayout->addWidget(boldButton);
    connect(boldButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::Bold);
    });

    auto italicButton = new QToolButton(this);
    italicButton->setObjectName(u"italicButton"_s);
    italicButton->setFocusPolicy(Qt::NoFocus);
    italicButton->setIconSize(QSize(12, 12));
    italicButton->setIcon(QIcon::fromTheme(u"format-text-italic"_s));
    italicButton->setAutoRaise(true);
    italicButton->setToolTip(i18nc("@info:tooltip", "Italic"));
    connect(italicButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::Italic);
    });
    mainLayout->addWidget(italicButton);

    auto strikeThroughButton = new QToolButton(this);
    strikeThroughButton->setObjectName(u"strikeThroughButton"_s);
    strikeThroughButton->setFocusPolicy(Qt::NoFocus);
    strikeThroughButton->setIconSize(QSize(12, 12));
    strikeThroughButton->setIcon(QIcon::fromTheme(u"format-text-strikethrough"_s));
    strikeThroughButton->setAutoRaise(true);
    strikeThroughButton->setToolTip(i18nc("@info:tooltip", "Strike Through"));
    connect(strikeThroughButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::StrikeThrough);
    });
    mainLayout->addWidget(strikeThroughButton);

    mainLayout->addWidget(new KSeparator(Qt::Vertical, this));

    auto codeBlockButton = new QToolButton(this);
    codeBlockButton->setObjectName(u"codeBlockButton"_s);
    codeBlockButton->setFocusPolicy(Qt::NoFocus);
    codeBlockButton->setIconSize(QSize(12, 12));
    codeBlockButton->setIcon(QIcon::fromTheme(u"format-text-code"_s));
    codeBlockButton->setToolTip(i18nc("@info:tooltip", "Code Block"));
    codeBlockButton->setAutoRaise(true);
    connect(codeBlockButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::CodeBlock);
    });

    mainLayout->addWidget(codeBlockButton);

    auto blockQuoteButton = new QToolButton(this);
    blockQuoteButton->setObjectName(u"blockQuoteButton"_s);
    blockQuoteButton->setFocusPolicy(Qt::NoFocus);
    blockQuoteButton->setIconSize(QSize(12, 12));
    blockQuoteButton->setIcon(QIcon::fromTheme(u"format-text-blockquote"_s));
    blockQuoteButton->setToolTip(i18nc("@info:tooltip", "Quote Text"));
    blockQuoteButton->setAutoRaise(true);
    connect(blockQuoteButton, &QToolButton::clicked, this, [this]() {
        Q_EMIT quickTextFormatRequested(QuickTextFormatMessage::QuickTextFormatType::BlockQuote);
    });

    mainLayout->addWidget(blockQuoteButton);

    mainLayout->addWidget(new KSeparator(Qt::Vertical, this));

    auto insertLinkButton = new QToolButton(this);
    insertLinkButton->setObjectName(u"insertLinkButton"_s);
    insertLinkButton->setFocusPolicy(Qt::NoFocus);
    insertLinkButton->setIconSize(QSize(12, 12));
    insertLinkButton->setIcon(QIcon::fromTheme(u"link"_s));
    insertLinkButton->setToolTip(i18nc("@info:tooltip", "Insert Link"));
    insertLinkButton->setAutoRaise(true);
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
