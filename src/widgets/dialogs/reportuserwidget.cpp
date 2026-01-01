/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportuserwidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mUserName->setObjectName(u"mUserName"_s);
    mUserName->setWordWrap(true);
    QFont messagePreviewFont = mUserName->font();
    messagePreviewFont.setBold(true);
    mUserName->setFont(messagePreviewFont);
    mainLayout->addWidget(mUserName);

    auto messageLayout = new QHBoxLayout;
    messageLayout->setObjectName(u"messageLayout"_s);
    messageLayout->setContentsMargins({});

    mainLayout->addLayout(messageLayout);

    auto lab = new QLabel(i18nc("@label:textbox", "Message:"), this);
    lab->setObjectName(u"label"_s);
    messageLayout->addWidget(lab, 0, Qt::AlignTop);

    mMessageLineEdit->setObjectName(u"mMessageLineEdit"_s);
    mMessageLineEdit->setAcceptRichText(false);
    mMessageLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Why do you want to report?"));
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
