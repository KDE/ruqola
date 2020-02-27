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

#include "reportmessagewidget.h"

#include <QLabel>
#include <QTextDocument>
#include <KTextEdit>
#include <QHBoxLayout>

#include <KLocalizedString>

ReportMessageWidget::ReportMessageWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mMessagePreview = new QLabel(this);
    mMessagePreview->setObjectName(QStringLiteral("mMessagePreview"));
    mMessagePreview->setWordWrap(true);
    QFont messagePreviewFont = mMessagePreview->font();
    messagePreviewFont.setBold(true);
    mMessagePreview->setFont(messagePreviewFont);
    mainLayout->addWidget(mMessagePreview);

    QHBoxLayout *messageLayout = new QHBoxLayout;
    messageLayout->setObjectName(QStringLiteral("messageLayout"));
    messageLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addLayout(messageLayout);

    QLabel *lab = new QLabel(i18n("Message:"), this);
    lab->setObjectName(QStringLiteral("label"));
    messageLayout->addWidget(lab);

    mMessageLineEdit = new KTextEdit(this);
    mMessageLineEdit->setObjectName(QStringLiteral("mMessageLineEdit"));
    mMessageLineEdit->setAcceptRichText(false);
    mMessageLineEdit->setPlaceholderText(i18n("Why you signal this message?"));
    messageLayout->addWidget(mMessageLineEdit);
    connect(mMessageLineEdit, &KTextEdit::textChanged, this, [this]() {
        Q_EMIT updateOkButton(!mMessageLineEdit->toPlainText().trimmed().isEmpty());
    });
}

ReportMessageWidget::~ReportMessageWidget()
{
}

QString ReportMessageWidget::message() const
{
    return mMessageLineEdit->toPlainText();
}

void ReportMessageWidget::setPreviewMessage(const QString &msg)
{
    mMessagePreview->setText(msg);
}
