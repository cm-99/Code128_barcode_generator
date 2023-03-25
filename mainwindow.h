#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GUI/barcodesettingsgui.h"
#include "GUI/filegenerationsettingsgui.h"
#include "GUI/expandablewidget.h"
#include "GUI/fixedbarcodepartgui.h"
#include "GUI/charactervariablebarcodepartgui.h"
#include "GUI/numbervariablebarcodepartgui.h"
#include "barcodeencoder.h"
#include "barcodeitem.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPrinter>
#include <QTextDocument>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * Main class of this project.
 * Creates and manages the UI, provides barcodes file generation functionality.
 * If the project was to develop further I would recommend separating the UI from file generation logic (FileGenerationManager?)
 * by creating another class (the division is already pretty well defined by the comments).
 * I might do it myself if requirements for barcodes/files generated will change AGAIN.
 *
 * Yes, I like classes with the word "Manager" in the name, how do you know :)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //UI attributes

    Ui::MainWindow *ui;
    BarcodeSettingsManager barcode_settings_manager;
    FileGenerationSettingsManager file_generation_settings_manager;

    QWidget central_widget;
    QVBoxLayout main_layout;
    QCheckBox silence_warnings_checkbox;

    BarcodeSettingsGUI barcode_settings_gui;
    QVBoxLayout *barcode_settings_layout;
    ExpandableWidget *barcode_settings_expandable_section;

    FileGenerationSettingsGUI file_generation_settings_gui;
    QVBoxLayout *file_generation_settings_layout;
    ExpandableWidget *file_generation_settings_expandable_section;

    QGraphicsView graphics_view;
    QGraphicsScene scene;

    QHBoxLayout barcode_parts_buttons_layout;
    QPushButton fixed_part_of_the_barcode_button;
    QPushButton character_variable_part_of_the_barcode_button;
    QPushButton number_variable_part_of_the_barcode_button;

    QVBoxLayout barcode_parts_layout;
    QProgressDialog *file_generation_progress_dialog;

    //Barcodes generation/file attributes

    QString current_barcode_text;
    QList<QString> barcode_text_parts;
    BarcodeItem *barcode_item;
    BarcodeEncoder *barcode_encoder;
    BarcodeType currently_used_barcode_type;

    QString file_name;
    QPrinter printer;
    QTextDocument barcodes_document;

    //Strictly UI oriented methods

    /**
     * Adds expandable widgets for settings sections.
     */
    void add_settings_sections();
    /**
     * Adds buttons for adding barcode parts editors.
     */
    void add_barcode_generation_buttons();
    /**
     * Adds barcode preview that is updated when user changes barcode settings.
     */
    void add_barcode_preview();
    /**
     * Updates preview. Checks if with new settings barcode fits the page size.
     * If not, QMessageBox with appropriate message is shown to the user.
     */
    void update_preview();
    /**
    * Attemps to get *.pdf file choice from the user through QFileDialog.
    * @return - returns name of the file chosen by the user. If user does not choose the file and exits QFileDialog it will return
    * empty QString. Such situation should be resolved by the caller.
    */
   QString get_file_for_barcodes_generation();
   /**
    * Attemps to get *.csv file choice from the user through QFileDialog.
    * @return - returns name of the file chosen by the user. If user does not choose the file and exits QFileDialog it will return
    * empty QString. Such situation should be resolved by the caller.
    */
   QString get_file_with_data_to_generate_as_barcodes();

    //Barcodes/file generation methods

    /**
     * Generates texts to be used as barcodes depending on provided fixed_barcode_part_gui.
     * Generation means here modification of already existing list_of_barcode_texts.
     * Number of elements in the list does not change unless the list was initially empty.
     */
    void generate_barcodes_for_fixed_barcode_part(FixedBarcodePartGUI *fixed_barcode_part_gui, QList<QString> *list_of_barcode_texts);
    /**
     * Generates texts to be used as barcodes depending on provided character_variable_barcode_part_gui.
     * Generation means here modification of already existing list_of_barcode_texts.
     * In case of non-zero steps value of character_variable_barcode_part_gui number of elements in the list rises.
     */
    void generate_barcodes_for_character_variable_barcode_part(CharacterVariableBarcodePartGUI *character_variable_barcode_part_gui, QList<QString> *list_of_barcode_texts);
    /**
     * Generates texts to be used as barcodes depending on provided number_variable_barcode_part_gui.
     * Generation means here modification of already existing list_of_barcode_texts.
     * In case of non-zero steps value of number_variable_barcode_part_gui number of elements in the list rises.
     */
    void generate_barcodes_for_number_variable_barcode_part(NumberVariableBarcodePartGUI *number_variable_barcode_part_gui, QList<QString> *list_of_barcode_texts);
    /**
     * Generates list of barcode texts using barcode parts provided by *BarcodePartGUI objects contained by barcode_parts_layout.
     * @return - returns list of generated barcode texts. It can be empty. Such situation should be resolved by the caller.
     */
    QList<QString> *generate_barcodes_list_from_parts();
    /**
     * Loads list of barcode texts from *.csv file chosen by the user.
     * @return - returns list of loaded barcode texts. It can be empty. Such situation should be resolved by the caller.
     */
    QList<QString> *load_barcodes_list_from_csv();


    //Some simple methods created for the sole purpose of better code readability

    void prepare_printer_settings(FileGenerationSettings &file_generation_settings);
    bool check_if_file_can_be_generated(int spacer_height, QList<QString> *list_of_barcode_texts);
    void insert_barcode_into_document(FileGenerationSettings current_file_generation_settings, int &rows_counter, int i, double horizontal_spacer_height_in_device_pixels, QTextCursor &cursor, QList<QString> *list_of_barcode_texts);
    void add_actions_to_menu();
    void generate_barcodes_file(QList<QString> *list_of_barcode_texts);

private slots:
    /**
     * Updates barcode_text_parts with new_barcode_text_part depending on whether calling *BarcodePartGUI
     * has already modified the list or not.
     * Updates barcode preview and current_barcode_text taking into account this modification.
     */
    void slot_update_barcode_text(QString new_barcode_text_part);
    /**
     * Updates barcode settings of barcode_item with new_barcode_settings.
     * Updates barcode preview.
     */
    void slot_update_barcode_settings(BarcodeSettings new_barcode_settings);
    /**
     * Generates barcodes in the file specified by the user.
     * Generated barcodes depend on *BarcodePartGUI objects added by the user to barcode_parts_layout.
     * Layout of the file depends on FileGenerationSettings set by the user through FileGenerationSettingsGUI.
     */
    void slot_generate_file_from_parts();
    /**
     * Generates barcodes in the file specified by the user.
     * Generated barcodes depend on the contents of *.csv file selected by the user.
     * Layout of the file depends on FileGenerationSettings set by the user through FileGenerationSettingsGUI.
     */
    void slot_generate_file_from_csv();
    /**
     * Adds FixedBarcodePartGUI object to barcode_parts_layout.
     */
    void slot_add_fixed_barcode_part_gui();
    /**
     * Adds CharacterVariableBarcodePartGUI object to barcode_parts_layout.
     */
    void slot_add_character_variable_barcode_part_gui();
    /**
     * Adds NumberVariableBarcodePartGUI object to barcode_parts_layout.
     */
    void slot_add_number_variable_barcode_part_gui();
    /**
     * Removes *BarcodePartGUI object which called this slot from the barcode_parts_layout.
     * Updates barcode's preview and current_barcode_text taking into account this removal.
     */
    void slot_remove_barcode_part();

};
#endif // MAINWINDOW_H
