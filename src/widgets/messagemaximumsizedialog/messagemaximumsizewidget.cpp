/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizewidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

MessageMaximumSizeWidget::MessageMaximumSizeWidget(QWidget *parent)
    : QWidget{parent}
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mFileName->setObjectName(u"mFileName"_s);
    mainLayout->addRow(i18n("Filename:"), mFileName);
    KLineEditEventHandler::catchReturnKey(mFileName);

    mDescription->setObjectName(u"mDescription"_s);
    mainLayout->addRow(i18n("Description:"), mDescription);
    KLineEditEventHandler::catchReturnKey(mDescription);

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
