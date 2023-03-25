#include "barcodesettingsmanager.h"

#include <QXmlStreamWriter>
#include <QDebug>

BarcodeSettings BarcodeSettingsManager::barcode_settings = BarcodeSettings();

BarcodeSettingsManager::BarcodeSettingsManager()
{
    QString settings_file_name = "barcode_settings.xml";
    QString settings_file_path = qApp->applicationDirPath() + "/" + settings_file_name;
    settings_file = new QFile(settings_file_path);

    //Check if settings file exists, create it if needed
    if(settings_file -> exists())
    {
        settings_file -> open(QIODevice::ReadWrite);
        load_settings_from_file();
    }
    else
        settings_file -> open(QIODevice::ReadWrite);
}

BarcodeSettingsManager::~BarcodeSettingsManager()
{
    settings_file -> close();
    delete settings_file;
}

BarcodeSettings BarcodeSettingsManager::get_barcode_settings()
{
    return barcode_settings;
}

void BarcodeSettingsManager::load_settings_from_file()
{
    QXmlStreamReader reader_XML(settings_file);

    reader_XML.readNextStartElement();
    while(reader_XML.name() != "BarcodeSettings" && reader_XML.error() == 0)
    {
        reader_XML.readNextStartElement();
    }

    if(reader_XML.name() == "BarcodeSettings")
    {
        reader_XML.readNextStartElement();
    }
    else{return;};

    if(reader_XML.name() == "barcode_type")
    {
        auto barcode_type = static_cast<BarcodeType>(reader_XML.readElementText().toInt());
        barcode_settings.barcode_type = barcode_type;
        reader_XML.readNextStartElement();
    }
    else{return;};

    if(reader_XML.name() == "text_is_visible")
    {
        barcode_settings.text_is_visible = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else{return;};

    if(reader_XML.name() == "text_is_to_be_divided")
    {
        barcode_settings.text_is_to_be_divided = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else {return;};

    if(reader_XML.name() == "barcode_width_scale")
    {
        barcode_settings.barcode_width_scale = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else {return;};

    if(reader_XML.name() == "barcode_height_in_mm")
    {
        barcode_settings.barcode_height_in_mm = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else {return;};

    if(reader_XML.name() == "font_size")
    {
        barcode_settings.font_size = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else {return;};

    if(reader_XML.name() == "text_part_size")
    {
        barcode_settings.text_part_size = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else {return;};
}

void BarcodeSettingsManager::save_settings_to_file(BarcodeSettings current_barcode_settings)
{
    settings_file->resize(0);
    QXmlStreamWriter writer_XML(settings_file);

    writer_XML.writeStartDocument();
    writer_XML.setAutoFormatting(true);
    writer_XML.setAutoFormattingIndent(4);

    writer_XML.writeStartElement("BarcodeSettings");
    writer_XML.writeTextElement("barcode_type", QString::number(current_barcode_settings.barcode_type));
    writer_XML.writeTextElement("text_is_visible", QString::number(current_barcode_settings.text_is_visible));
    writer_XML.writeTextElement("text_is_to_be_divided", QString::number(current_barcode_settings.text_is_to_be_divided));
    writer_XML.writeTextElement("barcode_width_scale", QString::number(current_barcode_settings.barcode_width_scale));
    writer_XML.writeTextElement("barcode_height_in_mm", QString::number(current_barcode_settings.barcode_height_in_mm));
    writer_XML.writeTextElement("font_size", QString::number(current_barcode_settings.font_size));
    writer_XML.writeTextElement("text_part_size", QString::number(current_barcode_settings.text_part_size));

    writer_XML.writeEndElement();
    writer_XML.writeEndDocument();
}
