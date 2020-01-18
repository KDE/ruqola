#include "messagelistdelegate.h"
#include "model/messagemodel.h"

#include <QPainter>

MessageListDelegate::MessageListDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);

    QRect displayRect = option.rect; //  for now
    //QRect decorationRect = opt.rect;

    const QString sender = index.data(MessageModel::Username).toString();
#if 0
    const QString iconFileName = mCache->avatarUrlFromCacheOnly(sender);
    QPixmap pixmap;
    if (!iconFileName.isEmpty()) {
        const bool loaded = pix.load(iconFileName); // SLOW! QPixmapCache anyone?
    }

    drawDecoration(painter, opt, decorationRect, pixmap);
#endif

    // First line: username
    const QString userText = QLatin1Char('@') + sender;
    const QFont font = option.font;
    QFont boldFont = font;
    boldFont.setBold(true);
    painter->setFont(boldFont);
    painter->drawText(displayRect, userText);
    painter->setFont(font);

    // Then the actual message (TODO richtext support)
    QRect messageRect = displayRect;
    messageRect.setTop(messageRect.top() + QFontMetrics(boldFont).height());
    const QString message = index.data(MessageModel::MessageConvertedText).toString();
    painter->drawText(messageRect, message);

    drawFocus(painter, option, displayRect);
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //QRect decorationRect = rect(option, index, Qt::DecorationRole);
    //QRect displayRect = d->displayRect(index, option, decorationRect, checkRect);

    QFont boldFont = option.font;
    boldFont.setBold(true);
    const int userHeight = QFontMetrics(boldFont).height();

    // TODO HACK
    return QSize(500, userHeight * 2);
}
