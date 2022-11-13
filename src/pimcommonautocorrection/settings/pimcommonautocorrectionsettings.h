/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimcommonautocorrection_export.h"
#include "pimcommonautocorrectionsetting_base.h"

class QTimer;

namespace PimCommonAutoCorrection
{
/**
 * @brief The PimCommonAutoCorrectionSettings class
 * @author Laurent Montel <montel@kde.org>
 */
class PIMCOMMONAUTOCORRECTION_EXPORT PimCommonAutoCorrectionSettings : public PimCommonAutoCorrection::PimCommonAutoCorrectionSettingsBase
{
    Q_OBJECT
public:
    static PimCommonAutoCorrectionSettings *self();

    /** Call this slot instead of directly @ref KConfig::sync() to
      minimize the overall config writes. Calling this slot will
      schedule a sync of the application config file using a timer, so
      that many consecutive calls can be condensed into a single
      sync, which is more efficient. */
    void requestSync();

private Q_SLOTS:
    void slotSyncNow();

private:
    PimCommonAutoCorrectionSettings();
    ~PimCommonAutoCorrectionSettings() override;
    static PimCommonAutoCorrectionSettings *mSelf;

    QTimer *mConfigSyncTimer = nullptr;
};
}
