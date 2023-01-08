/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "textautocorrectionsettings.h"
#include <QTimer>

using namespace TextAutoCorrection;

TextAutoCorrectionSettings *TextAutoCorrectionSettings::mSelf = nullptr;

TextAutoCorrectionSettings *TextAutoCorrectionSettings::self()
{
    if (!mSelf) {
        mSelf = new TextAutoCorrectionSettings();
        mSelf->load();
    }

    return mSelf;
}

TextAutoCorrectionSettings::TextAutoCorrectionSettings()
{
    mConfigSyncTimer = new QTimer(this);
    mConfigSyncTimer->setSingleShot(true);
    connect(mConfigSyncTimer, &QTimer::timeout, this, &TextAutoCorrectionSettings::slotSyncNow);
}

TextAutoCorrectionSettings::~TextAutoCorrectionSettings() = default;

void TextAutoCorrectionSettings::requestSync()
{
    if (!mConfigSyncTimer->isActive()) {
        mConfigSyncTimer->start(0);
    }
}

void TextAutoCorrectionSettings::slotSyncNow()
{
    config()->sync();
}
