/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "textemoticonswidgets_export.h"
#include <QWidget>
namespace TextEmoticonsCore
{
}
namespace TextEmoticonsWidgets
{
/**
 * @brief The EmoticonTextEditSelector class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTEMOTICONSWIDGETS_EXPORT EmoticonTextEditSelector : public QWidget
{
    Q_OBJECT
public:
    explicit EmoticonTextEditSelector(QWidget *parent = nullptr);
    ~EmoticonTextEditSelector() override;

    void forceLineEditFocus();

    void setCustomEmojiSupport(bool b);
    Q_REQUIRED_RESULT bool customEmojiSupport() const;

public Q_SLOTS:
    void loadEmoticons();

Q_SIGNALS:
    void insertEmoji(const QString &);
    void insertEmojiIdentifier(const QString &);

private:
    class EmoticonTextEditSelectorPrivate;
    std::unique_ptr<EmoticonTextEditSelectorPrivate> const d;
};
}
