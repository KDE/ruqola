/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "needupdateversionutils.h"
#include <KMessageWidget>
/**
 * @brief The NeedUpdateVersionWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT NeedUpdateVersionWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit NeedUpdateVersionWidget(QWidget *parent = nullptr);
    ~NeedUpdateVersionWidget() override;

    void setObsoleteVersion(NeedUpdateVersionUtils::ObsoleteVersion obsolete);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDisableVersionCheck();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCheckNewVersion();
};
