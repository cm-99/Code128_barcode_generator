#include "GUI/filegenerationsettingsgui.h"
#include "filegenerationsettingsmanager.h"

void FileGenerationSettingsGUI::create_input_widgets()
{
    page_orientation_combobox.addItem("Horizontal", Qt::Orientation::Horizontal);
    //Currently not supported. Squishing images.
    page_orientation_combobox.addItem("Vertical", Qt::Orientation::Vertical);
    page_orientation_combobox.setCurrentIndex(file_generation_settings.page_orientation - 1);
    settings_layout.addRow(tr("&Page orientation:"), &page_orientation_combobox);
    connect(&page_orientation_combobox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileGenerationSettingsGUI::slot_update_settings);

    rows_per_page_spinbox.setRange(1, 10);
    rows_per_page_spinbox.setValue(file_generation_settings.rows_per_page);
    settings_layout.addRow(tr("&Rows per page:"), &rows_per_page_spinbox);
    connect(&rows_per_page_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &FileGenerationSettingsGUI::slot_update_settings);

    page_height_in_mm_spinbox.setRange(0, 297);
    page_height_in_mm_spinbox.setValue(file_generation_settings.page_height_in_mm);
    settings_layout.addRow(tr("Page height [mm]:"), &page_height_in_mm_spinbox);
    connect(&page_height_in_mm_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &FileGenerationSettingsGUI::slot_update_settings);

    page_width_in_mm_spinbox.setRange(0, 210);
    page_width_in_mm_spinbox.setValue(file_generation_settings.page_width_in_mm);
    settings_layout.addRow(tr("Page width [mm]:"), &page_width_in_mm_spinbox);
    connect(&page_width_in_mm_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &FileGenerationSettingsGUI::slot_update_settings);
}

FileGenerationSettingsGUI::FileGenerationSettingsGUI(QWidget *parent) :
    QFrame{parent}
{
    file_generation_settings = FileGenerationSettingsManager::get_file_generation_settings();
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLayout(&settings_layout);

    create_input_widgets();

    file_generation_settings_spinboxes.append(&rows_per_page_spinbox);
    file_generation_settings_spinboxes.append(&page_height_in_mm_spinbox);
    file_generation_settings_spinboxes.append(&page_width_in_mm_spinbox);
}

FileGenerationSettingsGUI::~FileGenerationSettingsGUI(){}

FileGenerationSettings FileGenerationSettingsGUI::get_current_file_generation_settings()
{
    return file_generation_settings;
}

void FileGenerationSettingsGUI::slot_update_settings(int new_value)
{
    QObject *sender = QObject::sender();

    QSpinBox *possible_sender_spinbox = dynamic_cast<QSpinBox*>(sender);
    if(possible_sender_spinbox != nullptr)
    {
        if(file_generation_settings_spinboxes.contains(possible_sender_spinbox))
        {
            int index = file_generation_settings_spinboxes.indexOf(possible_sender_spinbox);
            if(index == 0)
            {
                file_generation_settings.rows_per_page = new_value;
            }
            else if(index == 1)
            {
                file_generation_settings.page_height_in_mm = new_value;
            }
            else if(index == 2)
            {
                file_generation_settings.page_width_in_mm = new_value;
            }
        }
        return;
    }

    QComboBox *possible_sender_combobox = dynamic_cast<QComboBox*>(sender);
    if(possible_sender_combobox != nullptr)
    {
        file_generation_settings.page_orientation = new_value + 1;
    }
}
