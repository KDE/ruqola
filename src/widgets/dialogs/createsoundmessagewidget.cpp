/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

CreateSoundMessageWidget::CreateSoundMessageWidget(QWidget *parent)
    : QWidget{parent}
    , mRecordButton(new QToolButton(this))
    , mLabelDuration(new QLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mRecordButton->setObjectName(QStringLiteral("mRecordButton"));
    mainLayout->addWidget(mRecordButton);
    mLabelDuration->setObjectName(QStringLiteral("mLabelDuration"));
    mLabelDuration->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(mLabelDuration);
    connect(mRecordButton, &QAbstractButton::clicked, this, &CreateSoundMessageWidget::slotRecord);
}

CreateSoundMessageWidget::~CreateSoundMessageWidget() = default;

void CreateSoundMessageWidget::slotRecord()
{
    // TODO
}
