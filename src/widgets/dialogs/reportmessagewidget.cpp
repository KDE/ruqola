/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagewidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KTextEdit>
#include <QHBoxLayout>
#include <QLabel>

#include <KLocalizedString>

ReportMessageWidget::ReportMessageWidget(QWidget *parent)
    : QWidget(parent)
    , mMessageLineEdit(new KTextEdit(this))
    , mMessagePreview(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mMessagePreview->setObjectName(u"mMessagePreview"_s);
    mMessagePreview->setWordWrap(true);
    QFont messagePreviewFont = mMessagePreview->font();
    messagePreviewFont.setBold(true);
    mMessagePreview->setFont(messagePreviewFont);
    mainLayout->addWidget(mMessagePreview);

    auto messageLayout = new QHBoxLayout;
    messageLayout->setObjectName(u"messageLayout"_s);
    messageLayout->setContentsMargins({});

    mainLayout->addLayout(messageLayout);

    auto lab = new QLabel(i18nc("@label:textbox", "Message:"), this);
    lab->setTextFormat(Qt::PlainText);
    lab->setObjectName(u"label"_s);
    messageLayout->addWidget(lab, 0, Qt::AlignTop);

    mMessageLineEdit->setObjectName(u"mMessageLineEdit"_s);
    mMessageLineEdit->setAcceptRichText(false);
    mMessageLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Why you signal this message?"));
    messageLayout->addWidget(mMessageLineEdit);
    connect(mMessageLineEdit, &KTextEdit::textChanged, this, [this]() {
        Q_EMIT updateOkButton(!mMessageLineEdit->toPlainText().trimmed().isEmpty());
    });
}

ReportMessageWidget::~ReportMessageWidget() = default;

QString ReportMessageWidget::message() const
{
    return mMessageLineEdit->toPlainText();
}

void ReportMessageWidget::setPreviewMessage(const QString &msg)
{
    mMessagePreview->setText(msg);
}

#include "moc_reportmessagewidget.cpp"
