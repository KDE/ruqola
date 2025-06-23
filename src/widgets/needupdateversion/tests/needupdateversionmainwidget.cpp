/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "needupdateversionmainwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "needupdateversion/needupdateversionutils.h"
#include "needupdateversion/needupdateversionwidget.h"
#include <QTextEdit>
#include <QVBoxLayout>

NeedUpdateVersionMainWidget::NeedUpdateVersionMainWidget(QWidget *parent)
    : QWidget{parent}
    , mNeedUpdateVersionWidget(new NeedUpdateVersionWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mainLayout->addWidget(mNeedUpdateVersionWidget);

    auto textEditor = new QTextEdit(this);
    mainLayout->addWidget(textEditor);
    checkVersion();
}

NeedUpdateVersionMainWidget::~NeedUpdateVersionMainWidget() = default;

void NeedUpdateVersionMainWidget::checkVersion()
{
    mNeedUpdateVersionWidget->setObsoleteVersion(NeedUpdateVersionUtils::obsoleteVersionStatus(u"22.04.1"_s, QDate::currentDate()));
}
#include "moc_needupdateversionmainwidget.cpp"
