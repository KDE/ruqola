/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportuserwidget.h"

#include <KTextEdit>
#include <QHBoxLayout>
#include <QLabel>

#include <KLocalizedString>

ReportUserWidget::ReportUserWidget(QWidget *parent)
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
    mMessageLineEdit->setPlaceholderText(i18n("Why do you want to report?"));
    messageLayout->addWidget(mMessageLineEdit);
    connect(mMessageLineEdit, &KTextEdit::textChanged, this, [this]() {
        Q_EMIT updateOkButton(!mMessageLineEdit->toPlainText().trimmed().isEmpty());
    });
}

ReportUserWidget::~ReportUserWidget() = default;

QString ReportUserWidget::message() const
{
    return mMessageLineEdit->toPlainText();
}

void ReportUserWidget::setUserName(const QString &userName)
{
    // TODO
}

#include "moc_reportuserwidget.cpp"
