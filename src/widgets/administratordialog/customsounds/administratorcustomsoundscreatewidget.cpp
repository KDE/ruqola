/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatewidget.h"
#include "misc/lineeditcatchreturnkey.h"
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
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mName->setObjectName(QStringLiteral("mName"));
    new LineEditCatchReturnKey(mName, this);

    mSelectFile->setObjectName(QStringLiteral("mSelectFile"));

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
