/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagewidget.h"

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
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mMessagePreview->setObjectName(QStringLiteral("mMessagePreview"));
    mMessagePreview->setWordWrap(true);
    QFont messagePreviewFont = mMessagePreview->font();
    messagePreviewFont.setBold(true);
    mMessagePreview->setFont(messagePreviewFont);
    mainLayout->addWidget(mMessagePreview);

    auto messageLayout = new QHBoxLayout;
    messageLayout->setObjectName(QStringLiteral("messageLayout"));
    messageLayout->setContentsMargins({});

    mainLayout->addLayout(messageLayout);

    auto lab = new QLabel(i18n("Message:"), this);
    lab->setObjectName(QStringLiteral("label"));
    messageLayout->addWidget(lab);

    mMessageLineEdit->setObjectName(QStringLiteral("mMessageLineEdit"));
    mMessageLineEdit->setAcceptRichText(false);
    mMessageLineEdit->setPlaceholderText(i18n("Why you signal this message?"));
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
