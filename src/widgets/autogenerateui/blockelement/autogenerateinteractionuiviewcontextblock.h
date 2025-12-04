/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/blockelement/autogenerateinteractionuiviewblockbase.h"
#include "libruqolawidgets_private_export.h"
class AutoGenerateInteractionUiViewElement;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewContextBlock : public AutoGenerateInteractionUiViewBlockBase
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewContextBlock(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewContextBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewContextBlock &other) const;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewElement *> elements() const;
    void setElements(const QList<AutoGenerateInteractionUiViewElement *> &newElements);

    void setErrorMessages(const QMap<QString, QString> &map) override;
    void assignState(const QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> &info) override;

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;
    [[nodiscard]] QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> serializeBlockState() const override;

private:
    QList<AutoGenerateInteractionUiViewElement *> mElements;
};

class QDebug;
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewElement &t);
