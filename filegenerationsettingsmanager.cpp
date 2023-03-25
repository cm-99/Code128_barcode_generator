#include "filegenerationsettingsmanager.h"

#include <QXmlStreamWriter>
#include <QDebug>

FileGenerationSettings FileGenerationSettingsManager::file_generation_settings = FileGenerationSettings();

FileGenerationSettingsManager::FileGenerationSettingsManager()
{
    QString settings_file_name = "file_generation_settings.xml";
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

FileGenerationSettingsManager::~FileGenerationSettingsManager()
{
    settings_file -> close();
    delete settings_file;
}

FileGenerationSettings FileGenerationSettingsManager::get_file_generation_settings()
{
    return file_generation_settings;
}

void FileGenerationSettingsManager::load_settings_from_file()
{
    QXmlStreamReader reader_XML(settings_file);

    reader_XML.readNextStartElement();
    while(reader_XML.name() != "FileGenerationSettings" && reader_XML.error() == 0)
    {
        reader_XML.readNextStartElement();
    }

    if(reader_XML.name() == "FileGenerationSettings")
    {
        reader_XML.readNextStartElement();
    }
    else{return;};

    if(reader_XML.name() == "page_orientation")
    {
        file_generation_settings.page_orientation = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else{return;};

    if(reader_XML.name() == "rows_per_page")
    {
        file_generation_settings.rows_per_page = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else {return;};

    if(reader_XML.name() == "page_height_in_mm")
    {
        file_generation_settings.page_height_in_mm = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else {return;};

    if(reader_XML.name() == "page_width_in_mm")
    {
        file_generation_settings.page_width_in_mm = reader_XML.readElementText().toInt();
        reader_XML.readNextStartElement();
    }
    else {return;};
}

void FileGenerationSettingsManager::save_settings_to_file(FileGenerationSettings current_file_generation_settings)
{
    settings_file->resize(0);
    QXmlStreamWriter writer_XML(settings_file);

    writer_XML.writeStartDocument();
    writer_XML.setAutoFormatting(true);
    writer_XML.setAutoFormattingIndent(4);
    writer_XML.writeStartElement("FileGenerationSettings");
    writer_XML.writeTextElement("page_orientation", QString::number(current_file_generation_settings.page_orientation));
    writer_XML.writeTextElement("rows_per_page", QString::number(current_file_generation_settings.rows_per_page));
    writer_XML.writeTextElement("page_height_in_mm", QString::number(current_file_generation_settings.page_height_in_mm));
    writer_XML.writeTextElement("page_width_in_mm", QString::number(current_file_generation_settings.page_width_in_mm));
    writer_XML.writeEndElement();
    writer_XML.writeEndDocument();
}

