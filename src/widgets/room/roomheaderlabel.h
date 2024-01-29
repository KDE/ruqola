/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QLabel>
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomHeaderLabel : public QLabel
{
    Q_OBJECT
public:
    explicit RoomHeaderLabel(QWidget *parent = nullptr);
    ~RoomHeaderLabel() override;

    void setRoomAnnouncement(const QString &announcement);
    void setRoomTopic(const QString &name);

    [[nodiscard]] const QString &fullText() const;

protected:
    void resizeEvent(QResizeEvent *ev) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMoreInfo(const QString &content);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateSqueezedText();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateHeaderText();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString rPixelSqueeze(const QString &text, int maxPixels) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QSize textSize(const QString &text) const;
    QString mFullText;
    QString mTopic;
    QString mAnnouncement;
    bool mExpandTopic = false;
};
