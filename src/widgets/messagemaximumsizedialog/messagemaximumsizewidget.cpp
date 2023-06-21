/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

MessageMaximumSizeWidget::MessageMaximumSizeWidget(QWidget *parent)
    : QWidget{parent}
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mFileName->setObjectName(QStringLiteral("mFileName"));
    mainLayout->addRow(i18n("Filename:"), mFileName);
    new LineEditCatchReturnKey(mFileName, this);

    mDescription->setObjectName(QStringLiteral("mDescription"));
    mainLayout->addRow(i18n("Description:"), mDescription);
    new LineEditCatchReturnKey(mDescription, this);

    connect(mFileName, &QLineEdit::textChanged, this, &MessageMaximumSizeWidget::slotTextChanged);
    connect(mDescription, &QLineEdit::textChanged, this, &MessageMaximumSizeWidget::slotTextChanged);
}

MessageMaximumSizeWidget::~MessageMaximumSizeWidget() = default;

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

#include "moc_messagemaximumsizewidget.cpp"
