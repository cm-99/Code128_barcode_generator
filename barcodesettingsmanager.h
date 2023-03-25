#ifndef BARCODESETTINGSMANAGER_H
#define BARCODESETTINGSMANAGER_H

#include <QString>
#include <QApplication>
#include <QFile>

typedef enum {Code128 = 0} BarcodeType;

struct BarcodeSettings
{
    BarcodeType barcode_type;
    bool text_is_visible;
    /**
     * Text under the barcode can be divided into parts separated by spaces if this variable is set to true.
     */
    bool text_is_to_be_divided;
    int barcode_width_scale;
    int barcode_height_in_mm;
    int font_size;
    /**
     * Size of a single part of barcodes text if division into parts is to be used.
     */
    int text_part_size;

    /** Default constructor */
    explicit BarcodeSettings()
    {
        this -> barcode_type = BarcodeType::Code128;
        this -> text_is_visible = true;
        this -> text_is_to_be_divided = false;
        this -> barcode_width_scale = 1;
        this -> barcode_height_in_mm = 15;
        this -> font_size = 12;
        this -> text_part_size = 2;
    }

    /** Full constructor */
    BarcodeSettings(BarcodeType barcode_type, bool text_is_visible, bool text_is_to_be_divided, int barcode_width_scale, int barcode_height_in_mm, int font_size, int text_part_size)
    {
        this -> barcode_type = barcode_type;
        this -> text_is_visible = text_is_visible;
        this -> text_is_to_be_divided = text_is_to_be_divided;
        this -> barcode_width_scale = barcode_width_scale;
        this -> barcode_height_in_mm = barcode_height_in_mm;
        this -> font_size = font_size;
        this -> text_part_size = text_part_size;
    }

    virtual ~BarcodeSettings(){}
};
Q_DECLARE_METATYPE(BarcodeSettings)

/**
 * Class managing barcode settings -
 * mainly loading/saving them from/to file, providing access point to those settings for other classes through static method.
 */
class BarcodeSettingsManager
{
public:
    BarcodeSettingsManager();
    ~BarcodeSettingsManager();
    static BarcodeSettings get_barcode_settings();
    /**
     * Saves settings passed as @param:current_barcode_settings to the default settings_file.
     * To be used at application exit time.
     */
    void save_settings_to_file(BarcodeSettings current_barcode_settings);

private:
    QFile *settings_file;
    static BarcodeSettings barcode_settings;

    /**
     * Loads barcode settings from *.xml file.
     */
    void load_settings_from_file();

};


#endif // BARCODESETTINGSMANAGER_H
