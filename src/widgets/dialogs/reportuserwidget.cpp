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
    , mUserName(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setWordWrap(true);
    QFont messagePreviewFont = mUserName->font();
    messagePreviewFont.setBold(true);
    mUserName->setFont(messagePreviewFont);
    mainLayout->addWidget(mUserName);

    auto messageLayout = new QHBoxLayout;
    messageLayout->setObjectName(QStringLiteral("messageLayout"));
    messageLayout->setContentsMargins({});

    mainLayout->addLayout(messageLayout);

    auto lab = new QLabel(i18n("Message:"), this);
    lab->setObjectName(QStringLiteral("label"));
    messageLayout->addWidget(lab, 0, Qt::AlignTop);

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
    mUserName->setText(userName);
}

#include "moc_reportuserwidget.cpp"
