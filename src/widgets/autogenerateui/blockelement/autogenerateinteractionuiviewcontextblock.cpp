/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewcontextblock.h"
#include "autogenerateui/autogenerateinteractionuiviewimage.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonArray>
#include <QLabel>
#include <QLayout>
#include <QWidget>
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
    return AutoGenerateInteractionUiViewBlockBase::operator==(other) && other.elements() == elements();
}

QWidget *AutoGenerateInteractionUiViewContextBlock::generateWidget(QWidget *parent) const
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    widget->setLayout(hboxLayout);
    for (auto e : std::as_const(mElements)) {
        if (auto f = dynamic_cast<AutoGenerateInteractionUiViewText *>(e)) {
            auto label = new QLabel(parent);
            label->setWordWrap(true);
            label->setText(f->generateText());
            label->setOpenExternalLinks(true);
            hboxLayout->addWidget(label);
        } else if (auto f = dynamic_cast<AutoGenerateInteractionUiViewImage *>(e)) {
            // TODO
        }
    }
    return widget;
}

void AutoGenerateInteractionUiViewContextBlock::parseBlock(const QJsonObject &json)
{
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

void AutoGenerateInteractionUiViewContextBlock::serializeBlock(QJsonObject &o) const
{
    QJsonArray r;
    for (const auto &e : mElements) {
        r.append(e->serialize());
    }
    // TODO fix me
    o["<NEED>"_L1] = r;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewContextBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    d.space() << "elements:" << t.elements();
    return d;
}
