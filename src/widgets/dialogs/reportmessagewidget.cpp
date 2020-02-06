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

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include <KLocalizedString>

ReportMessageWidget::ReportMessageWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *lab = new QLabel(i18n("Message:"), this);
    lab->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(lab);

    mMessageLineEdit = new QLineEdit(this);
    mMessageLineEdit->setObjectName(QStringLiteral("mMessageLineEdit"));
    mMessageLineEdit->setClearButtonEnabled(true);
    mMessageLineEdit->setPlaceholderText(i18n("Why you signal this message?"));
    mainLayout->addWidget(mMessageLineEdit);
    connect(mMessageLineEdit, &QLineEdit::textChanged, this, [this]() {
        Q_EMIT updateOkButton(!mMessageLineEdit->text().trimmed().isEmpty());
    });
}

ReportMessageWidget::~ReportMessageWidget()
{
}

QString ReportMessageWidget::message() const
{
    return mMessageLineEdit->text();
}
