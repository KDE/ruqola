/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showimageprevnextimagewidget.h"
#include <QHBoxLayout>
#include <QToolButton>
ShowImagePrevNextImageWidget::ShowImagePrevNextImageWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto previousButton = new QToolButton(this);
    previousButton->setIcon(QIcon::fromTheme(QStringLiteral("go-previous")));
    previousButton->setEnabled(false);
    connect(previousButton, &QToolButton::triggered, this, []() { });

    auto nextButton = new QToolButton(this);
    nextButton->setIcon(QIcon::fromTheme(QStringLiteral("nextButton")));
    nextButton->setEnabled(false);
    connect(nextButton, &QToolButton::triggered, this, []() { });

    mainLayout->addWidget(previousButton);
    mainLayout->addWidget(nextButton);

    connect(this, &ShowImagePrevNextImageWidget::updateButton, this, [this, previousButton, nextButton](bool hasPrevious, bool hasNext) {
        previousButton->setEnabled(hasPrevious);
        nextButton->setEnabled(hasNext);
    });
}

ShowImagePrevNextImageWidget::~ShowImagePrevNextImageWidget() = default;

#include "moc_showimageprevnextimagewidget.cpp"
