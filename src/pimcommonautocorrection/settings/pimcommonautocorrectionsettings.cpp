/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimcommonautocorrectionsettings.h"
#include <QTimer>

using namespace PimCommonAutoCorrection;

PimCommonAutoCorrectionSettings *PimCommonAutoCorrectionSettings::mSelf = nullptr;

PimCommonAutoCorrectionSettings *PimCommonAutoCorrectionSettings::self()
{
    if (!mSelf) {
        mSelf = new PimCommonAutoCorrectionSettings();
        mSelf->load();
    }

    return mSelf;
}

PimCommonAutoCorrectionSettings::PimCommonAutoCorrectionSettings()
{
    mConfigSyncTimer = new QTimer(this);
    mConfigSyncTimer->setSingleShot(true);
    connect(mConfigSyncTimer, &QTimer::timeout, this, &PimCommonAutoCorrectionSettings::slotSyncNow);
}

PimCommonAutoCorrectionSettings::~PimCommonAutoCorrectionSettings() = default;

void PimCommonAutoCorrectionSettings::requestSync()
{
    if (!mConfigSyncTimer->isActive()) {
        mConfigSyncTimer->start(0);
    }
}

void PimCommonAutoCorrectionSettings::slotSyncNow()
{
    config()->sync();
}
