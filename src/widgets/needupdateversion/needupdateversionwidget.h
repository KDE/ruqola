/*
  SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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
    void slotDisableVersionCheck();
};
