#include "code128barcodeitem.h"

#include <QPainter>
#include <QDebug>

Code128BarcodeItem::Code128BarcodeItem() :
    visible_text_width(0),
    visible_text_height(0),
    barcode_width(200),
    barcode_height(100),
    barcode_modules_count(0)
{
    barcode_settings = BarcodeSettingsManager::get_barcode_settings();
    barcode_text_font = QFont("Times", barcode_settings.font_size, QFont::Bold);
}

Code128BarcodeItem::Code128BarcodeItem(BarcodeSettings barcode_settings) :
    visible_text_width(0),
    visible_text_height(0),
    barcode_width(200),
    barcode_height(100),
    barcode_modules_count(0)
{
    this -> barcode_settings = barcode_settings;
    barcode_text_font = QFont("Times", barcode_settings.font_size, QFont::Bold);
}

void Code128BarcodeItem::prepare_barcode_with_new_text(const QString &text, BarcodeEncoder *barcode_encoder)
{
    bar_widths = barcode_encoder->encode(text);
    barcode_text = text;
    barcode_visible_text = text;

    barcode_modules_count = 0;
    for(int i = 0; i < bar_widths.length(); i++)
    {
        barcode_modules_count += bar_widths[i];
    }
}

void Code128BarcodeItem::set_barcode_settings(BarcodeSettings new_barcode_settings)
{
    this -> barcode_settings = new_barcode_settings;
    barcode_text_font.setPointSize(barcode_settings.font_size);
}

void Code128BarcodeItem::set_screen_resolution(int screen_resolution)
{
    this -> screen_resolution = screen_resolution;
}

QRectF Code128BarcodeItem::boundingRect() const
{
    int barcode_item_width = barcode_width;
    if(visible_text_width > barcode_width)
    {
        barcode_item_width = visible_text_width;
    }

    int barcode_item_height = barcode_height + visible_text_height;
    return QRectF(0,0, barcode_item_width, barcode_item_height);
}

QString Code128BarcodeItem::divide_text_into_parts()
{
    QString text_separated;
    int current_part_characters_counter = 0;

    for(int i = 0; i < barcode_text.length(); i++)
    {
        text_separated += barcode_text[i];
        current_part_characters_counter ++;

        //Start adding new part of the text if current one is complete.
        //Do not add extra space if last character of the text completed the part.
        if(current_part_characters_counter == barcode_settings.text_part_size && i != barcode_text.length() - 1)
        {
            text_separated += " ";
            current_part_characters_counter = 0;
        }
    }

    return text_separated;
}

//Paints the barcode item
void Code128BarcodeItem::paint(QPainter *painter,[[maybe_unused]] const QStyleOptionGraphicsItem *option,[[maybe_unused]] QWidget *widget)
{
    int module_width = barcode_settings.barcode_width_scale;

    //Calculate barcode dimensions
    if(barcode_settings.barcode_width_scale != 0)
    {
        barcode_width = module_width * barcode_modules_count;
        barcode_height = (barcode_settings.barcode_height_in_mm*screen_resolution)/25.4;
    }
    else //Option to create text only
    {
        barcode_width = 0;
        barcode_height = 0;
    }

    //Calculate visible text's dimensions and prepare it to be created

    //If width of text is higher than that of barcode, barcode needs to be shifted right by half of the differential
    //It is needed to ensure text's center alignment and proper conversion to image
    qreal barcode_offset = 0;
    int text_box_width = 0;
    barcode_visible_text = barcode_text;

    if(barcode_settings.text_is_visible)
    {
        painter->setFont(barcode_text_font);

        if(barcode_settings.text_is_to_be_divided)
            barcode_visible_text = divide_text_into_parts();

        QFontMetrics font_metrics(barcode_text_font);

        visible_text_width = font_metrics.horizontalAdvance(barcode_visible_text);
        visible_text_height = font_metrics.height();
        text_box_width = visible_text_width;

        if(barcode_width > visible_text_width)
        {
            text_box_width = barcode_width;
        }
        else
        {
            barcode_offset = (visible_text_width - barcode_width)/2;
        }
    }

    int current_barcodes_x_edge = barcode_offset;

    if(barcode_settings.barcode_width_scale != 0)
    {
        //Create barcode as QRectF
        for(int i = 0; i < bar_widths.length(); i++)
        {
            int new_bars_width = bar_widths[i] * module_width;

            //Create only black bars, omit empty ones
            if (i % 2 == 0)
            {
                QRectF bar(current_barcodes_x_edge, 0, new_bars_width, barcode_height);
                painter -> fillRect(bar, Qt::SolidPattern);
            }
            current_barcodes_x_edge += new_bars_width;
        }
    }

    //Create visible text
    if(barcode_settings.text_is_visible)
    {
        QRectF text_box(0, barcode_height, text_box_width, visible_text_height);
        QTextOption text_options = Qt::AlignHCenter|Qt::AlignVCenter;
        painter->drawText(text_box, barcode_visible_text, text_options);
    }
}
