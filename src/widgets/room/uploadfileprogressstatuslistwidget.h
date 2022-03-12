/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QMap>
#include <QScrollArea>
class UploadFileProgressStatusWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UploadFileProgressStatusListWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit UploadFileProgressStatusListWidget(QWidget *parent = nullptr);
    ~UploadFileProgressStatusListWidget() override;

    Q_REQUIRED_RESULT UploadFileProgressStatusWidget *addProgressStatusWidget(int identifier, bool first);

protected:
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    void slotLayoutFirstItem();
    void removeUploadFileProgressStatusWidget();
    QWidget *const mBigBox;
    QMap<int, UploadFileProgressStatusWidget *> mUploadItems;
};
