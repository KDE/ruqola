/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

MessageMaximumSizeWidget::MessageMaximumSizeWidget(QWidget *parent)
    : QWidget{parent}
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mFileName->setObjectName(QStringLiteral("mFileName"));
    mainLayout->addWidget(mFileName);
    new LineEditCatchReturnKey(mFileName, this);

    mDescription->setObjectName(QStringLiteral("mDescription"));
    mainLayout->addWidget(mDescription);
    new LineEditCatchReturnKey(mDescription, this);

    connect(mFileName, &QLineEdit::textChanged, this, &MessageMaximumSizeWidget::slotTextChanged);
    connect(mDescription, &QLineEdit::textChanged, this, &MessageMaximumSizeWidget::slotTextChanged);
}

MessageMaximumSizeWidget::~MessageMaximumSizeWidget()
{
}

void MessageMaximumSizeWidget::slotTextChanged()
{
    Q_EMIT updateOkButton(!mFileName->text().trimmed().isEmpty() && !mDescription->text().trimmed().isEmpty());
}

QString MessageMaximumSizeWidget::description() const
{
    return mDescription->text();
}

QString MessageMaximumSizeWidget::fileName() const
{
    return mFileName->text();
}
