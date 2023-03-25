#ifndef BARCODEITEM_H
#define BARCODEITEM_H

#include <QGraphicsItem>

#include "barcodeencoder.h"
#include "barcodesettingsmanager.h"

/**
 * Interface for barcode items, where a barcode item is a barcode + text under it.
 * For implementation guidelines and methods description see Code128BarcodeItem.
 */
class BarcodeItem: public QGraphicsItem
{
public:
    virtual ~BarcodeItem(){};

    virtual void set_barcode_settings(BarcodeSettings new_barcode_settings) = 0;
    virtual void set_screen_resolution(int screen_resolution) = 0;

    virtual QRectF boundingRect() const = 0;
    virtual void prepare_barcode_with_new_text(const QString &text, BarcodeEncoder *barcode_encoder) = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget) = 0;
};

#endif // BARCODEITEM_H
