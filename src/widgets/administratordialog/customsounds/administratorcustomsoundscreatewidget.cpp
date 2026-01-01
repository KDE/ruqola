/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatewidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KUrlRequester>
#include <QFormLayout>
#include <QLineEdit>

AdministratorCustomSoundsCreateWidget::AdministratorCustomSoundsCreateWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mSelectFile(new KUrlRequester(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mName->setObjectName(u"mName"_s);
    KLineEditEventHandler::catchReturnKey(mName);

    mSelectFile->setObjectName(u"mSelectFile"_s);

    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("File:"), mSelectFile);
}

AdministratorCustomSoundsCreateWidget::~AdministratorCustomSoundsCreateWidget() = default;

void AdministratorCustomSoundsCreateWidget::setCustomSoundInfo(const CustomSoundInfo &info)
{
    mSelectFile->setUrl(info.fileNameUrl);
    mName->setText(info.name);
}

AdministratorCustomSoundsCreateWidget::CustomSoundInfo AdministratorCustomSoundsCreateWidget::customSoundInfo() const
{
    AdministratorCustomSoundsCreateWidget::CustomSoundInfo info;
    info.fileNameUrl = mSelectFile->url();
    info.name = mName->text().trimmed();
    return info;
}

#include "moc_administratorcustomsoundscreatewidget.cpp"
