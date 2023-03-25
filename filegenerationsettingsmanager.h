#ifndef FILEGENERATIONSETTINGSMANAGER_H
#define FILEGENERATIONSETTINGSMANAGER_H

#include <QString>
#include <QApplication>
#include <QFile>

struct FileGenerationSettings
{
    int page_orientation;
    int rows_per_page;
    int page_height_in_mm;
    int page_width_in_mm;

    /** Default constructor */
    explicit FileGenerationSettings()
    {
        this -> page_orientation = Qt::Orientation::Horizontal;
        this -> rows_per_page = 3;
        this -> page_height_in_mm = 297;
        this -> page_width_in_mm = 210;
    }

    /** Full constructor */
    FileGenerationSettings(int page_orientation, int rows_per_page, int page_height_in_mm, int page_width_in_mm)
    {
        this -> page_orientation = page_orientation;
        this -> rows_per_page = rows_per_page;
        this -> page_height_in_mm = page_height_in_mm;
        this -> page_width_in_mm = page_width_in_mm;
    }

    virtual ~FileGenerationSettings(){}
};
Q_DECLARE_METATYPE(FileGenerationSettings)

//A common interface with BarcodeSettingsManager (SettingsManager?) could be extracted.

/**
 * Class managing file generation settings -
 * mainly loading/saving them from/to file, providing access point to those settings for other classes.
 */
class FileGenerationSettingsManager
{
public:
    FileGenerationSettingsManager();
    ~FileGenerationSettingsManager();
    static FileGenerationSettings get_file_generation_settings();

    /**
     * Saves settings passed as @param:current_file_generation_settings.
     * To be used at application exit time.
     */
    void save_settings_to_file(FileGenerationSettings current_file_generation_settings);

private:
    QFile *settings_file;
    static FileGenerationSettings file_generation_settings;

    /**
     * Loads file generation settings from *.xml file.
     */
    void load_settings_from_file();
};

#endif // FILEGENERATIONSETTINGSMANAGER_H
