/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatewidget.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KUrlRequester>
#include <QFormLayout>
#include <QLineEdit>

using namespace Qt::Literals::StringLiterals;
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
    mSelectFile->setNameFilters({QStringLiteral("%1 (*.mp3)").arg(i18nc("@item:inlistbox File type selection filter", "mp3 file"))});
    // TODO add more ?

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
