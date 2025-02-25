/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewcontextblock.h"
#include "autogenerateui/autogenerateinteractionuiviewimage.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewContextBlock::AutoGenerateInteractionUiViewContextBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewContextBlock::~AutoGenerateInteractionUiViewContextBlock()
{
    qDeleteAll(mElements);
}

bool AutoGenerateInteractionUiViewContextBlock::operator==(const AutoGenerateInteractionUiViewContextBlock &other) const
{
    return other.elements() == elements();
}

QWidget *AutoGenerateInteractionUiViewContextBlock::generateWidget(QWidget *parent) const
{
    // TODO
    return nullptr;
}

void AutoGenerateInteractionUiViewContextBlock::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewBlockBase::parse(json);
    for (const auto &r : json["elements"_L1].toArray()) {
        const QString type = r["type"_L1].toString();
        if (type == "plain_text"_L1 || type == "mrkdwn"_L1) {
            AutoGenerateInteractionUiViewText *text = new AutoGenerateInteractionUiViewText;
            text->parse(r.toObject());
            mElements.append(std::move(text));
        } else if (type == "image"_L1) {
            AutoGenerateInteractionUiViewImage *img = new AutoGenerateInteractionUiViewImage;
            img->parse(r.toObject());
            mElements.append(std::move(img));
        } else {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock Unknown type " << type;
        }
    }
}

QList<AutoGenerateInteractionUiViewElement *> AutoGenerateInteractionUiViewContextBlock::elements() const
{
    return mElements;
}

void AutoGenerateInteractionUiViewContextBlock::setElements(const QList<AutoGenerateInteractionUiViewElement *> &newElements)
{
    mElements = newElements;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewContextBlock &t)
{
    d.space() << "elements:" << t.elements();
    return d;
}
