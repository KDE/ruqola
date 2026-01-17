/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showimageprevnextimagewidget.h"

#include <KLocalizedString>
#include <QHBoxLayout>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
ShowImagePrevNextImageWidget::ShowImagePrevNextImageWidget(QWidget *parent)
    : QWidget{parent}
    , mNextButton(new QToolButton(this))
    , mPreviousButton(new QToolButton(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mPreviousButton->setIcon(QIcon::fromTheme(u"go-previous"_s));
    mPreviousButton->setEnabled(false);
    mPreviousButton->setToolTip(i18n("Show previous image"));
    mPreviousButton->setObjectName(u"mPreviousButton"_s);
    connect(mPreviousButton, &QToolButton::clicked, this, &ShowImagePrevNextImageWidget::showPreviousImage);

    mNextButton->setIcon(QIcon::fromTheme(u"nextButton"_s));
    mNextButton->setEnabled(false);
    mNextButton->setObjectName(u"mNextButton"_s);
    mNextButton->setIcon(QIcon::fromTheme(u"go-next"_s));
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
