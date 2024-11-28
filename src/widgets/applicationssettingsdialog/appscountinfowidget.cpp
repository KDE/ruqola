/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfowidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QProgressBar>
using namespace Qt::Literals::StringLiterals;
AppsCountInfoWidget::AppsCountInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mProgressBar(new QProgressBar(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mProgressBar->setObjectName("mProgressBar"_L1);
    mainLayout->addWidget(mProgressBar);
}

AppsCountInfoWidget::~AppsCountInfoWidget() = default;

void AppsCountInfoWidget::setAppCountInfo(const AppsCountInfo &info)
{
    setVisible(info.isValid());
    // TODO
}

#include "moc_appscountinfowidget.cpp"
