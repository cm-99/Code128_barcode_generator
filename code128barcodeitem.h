#ifndef CODE128BARCODEITEM_H
#define CODE128BARCODEITEM_H

#include "barcodeencoder.h"
#include "barcodeitem.h"
#include "barcodesettingsmanager.h"

#include <QFont>

/**
 * Implementation of BarcodeItem for Code128 barcode.
 */
class Code128BarcodeItem : public BarcodeItem
{
public:
    Code128BarcodeItem();
    Code128BarcodeItem(BarcodeSettings barcode_settings);

    void set_barcode_settings(BarcodeSettings new_barcode_settings) override;
    void set_screen_resolution(int screen_resolution) override;

    /**
     * Prepares attributes needed for barcode painting.
     * Barcode encoding is called here for new @param:text using @param:barcode_encoder.
     *
     * prepare_barcode_with_new_text and paint are kept separated to divide the scenario where only barcode settings has changed
     * from the scenario where barcode text has changed.
     */
    void prepare_barcode_with_new_text(const QString &text, BarcodeEncoder *barcode_encoder) override;
    /**
     * @return - returns the smallest QRectF that barcode item could fit into.
     */
    QRectF boundingRect() const override;
    /**
     * Paints the barcode item according to barcode_settings, using @param:painter.
     * Other arguments are not currently in use.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget) override;

private:
    BarcodeSettings barcode_settings;
    int screen_resolution;

    QString barcode_text;
    QString barcode_visible_text;
    QFont barcode_text_font;
    int visible_text_width;
    int visible_text_height;

    int barcode_width;
    /**
     * Height of barcode without text under it (barcode != barcode item).
     */
    double barcode_height;
    /**
     * Number of single module wide bars forming the barcode.
     */
    int barcode_modules_count;
    /**
     * List of bar widths forming the barcode.
     */
    QList<int> bar_widths;
    /**
     * Divides barcode_text according to barcode_settings.text_part_size.
     * @return - returns resulting QString.
     */
    QString divide_text_into_parts();
};

#endif // CODE128BARCODEITEM_H
