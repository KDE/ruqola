/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showimageprevnextimagewidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QToolButton>
ShowImagePrevNextImageWidget::ShowImagePrevNextImageWidget(QWidget *parent)
    : QWidget{parent}
    , mNextButton(new QToolButton(this))
    , mPreviousButton(new QToolButton(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mPreviousButton->setIcon(QIcon::fromTheme(QStringLiteral("go-previous")));
    mPreviousButton->setEnabled(false);
    mPreviousButton->setToolTip(i18n("Show previous image"));
    mPreviousButton->setObjectName(QStringLiteral("mPreviousButton"));
    connect(mPreviousButton, &QToolButton::clicked, this, &ShowImagePrevNextImageWidget::showPreviousImage);

    mNextButton->setIcon(QIcon::fromTheme(QStringLiteral("nextButton")));
    mNextButton->setEnabled(false);
    mNextButton->setObjectName(QStringLiteral("mNextButton"));
    mNextButton->setIcon(QIcon::fromTheme(QStringLiteral("go-next")));
    mNextButton->setToolTip(i18n("Show next image"));
    connect(mNextButton, &QToolButton::clicked, this, &ShowImagePrevNextImageWidget::showNextImage);

    mainLayout->addWidget(mPreviousButton);
    mainLayout->addWidget(mNextButton);
}

ShowImagePrevNextImageWidget::~ShowImagePrevNextImageWidget() = default;

void ShowImagePrevNextImageWidget::setUpdateButtons(bool hasPrevious, bool hasNext)
{
    mPreviousButton->setEnabled(hasPrevious);
    mNextButton->setEnabled(hasNext);
}

#include "moc_showimageprevnextimagewidget.cpp"
