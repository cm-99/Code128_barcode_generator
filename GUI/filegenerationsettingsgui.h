#ifndef FILEGENERATIONSETTINGSGUI_H
#define FILEGENERATIONSETTINGSGUI_H

#include "filegenerationsettingsmanager.h"

#include <QFrame>
#include <QObject>
#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>

/**
 * GUI for changing file generation settings.
 */
class FileGenerationSettingsGUI : public QFrame
{
    Q_OBJECT
public:
    explicit FileGenerationSettingsGUI(QWidget *parent = nullptr);
    ~FileGenerationSettingsGUI();
    /**
     * Returns FileGenerationSettings struct holding current settings modified through this class.
     */
    FileGenerationSettings get_current_file_generation_settings();

private:
    /**
     * Current file settings, keeped up to date through slot_update_settings.
     */
    FileGenerationSettings file_generation_settings;

    //UI attributes (mainly input widgets)

    QFormLayout settings_layout;

    QComboBox page_orientation_combobox;
    QSpinBox rows_per_page_spinbox;
    QSpinBox page_height_in_mm_spinbox;
    QSpinBox page_width_in_mm_spinbox;

    QList<QSpinBox*> file_generation_settings_spinboxes;

    //Methods

    /**
     * Creates input widgets for file_generation_settings adjustment, sets their initial values to those from current settings.
     * Connects all inputs to slot_update_settings.
     */
    void create_input_widgets();

private slots:
    /**
     * Updates file_generation_settings in response to settings modification through input widgets.
     */
    void slot_update_settings(int new_value);

};

#endif // FILEGENERATIONSETTINGSGUI_H
