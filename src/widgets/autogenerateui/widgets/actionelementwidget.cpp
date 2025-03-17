/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "actionelementwidget.h"
#include <QLabel>
#include <QVBoxLayout>

ActionElementWidget::ActionElementWidget(QWidget *parent)
    : QWidget{parent}
    , mErrorLabel(new QLabel(this))
    , mLayout(new QHBoxLayout(this))
{
    mLayout->setContentsMargins({});
    mLayout->addWidget(mErrorLabel);
    // Hide by default
    mErrorLabel->setVisible(false);
}

ActionElementWidget::~ActionElementWidget() = default;

void ActionElementWidget::clearError()
{
    mErrorLabel->clear();
    mErrorLabel->setVisible(false);
}

void ActionElementWidget::setErrorMessages(const QString &msg)
{
    mErrorLabel->setVisible(true);
    mErrorLabel->setText(msg);
}
