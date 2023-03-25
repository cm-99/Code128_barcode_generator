#include "mainwindow.h"
#include "ui_mainwindow.h"

//Implementations of barcode encoder and barcode item
#include "code128barcodeitem.h"
#include "code128encoder.h"

#include <QDebug>
#include <QTextCursor>
#include <QMessageBox>
#include <QTextEdit>
#include <QTextBlock>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    barcode_settings_layout(new QVBoxLayout),
    barcode_settings_expandable_section(new ExpandableWidget("Barcode settings", 300, this)),
    file_generation_settings_layout(new QVBoxLayout),
    file_generation_settings_expandable_section(new ExpandableWidget("File generation settings", 300, this)),
    current_barcode_text("")
{
    ui->setupUi(this);
    add_actions_to_menu();

    setCentralWidget(&central_widget);
    central_widget.setLayout(&main_layout);

    silence_warnings_checkbox.setText("Silence barcode preview warnings");
    main_layout.addWidget(&silence_warnings_checkbox);

    add_settings_sections();

    //Add graphics view
    graphics_view.setScene(&scene);
    main_layout.addWidget(&graphics_view);

    //Choose right barcode_encoder
    currently_used_barcode_type = barcode_settings_manager.get_barcode_settings().barcode_type;
    if(currently_used_barcode_type == BarcodeType::Code128)
    {
        barcode_encoder = new Code128Encoder();
    }
    else
        barcode_encoder = new Code128Encoder(); //Always initialize encoder, even if a wrong one

    //Create initial barcode and page outline
    add_barcode_preview();

    //Prepare rest of the layouts
    main_layout.addLayout(&barcode_parts_buttons_layout);
    main_layout.addLayout(&barcode_parts_layout);

    //Add buttons for adding barcode parts
    add_barcode_generation_buttons();
}

MainWindow::~MainWindow()
{
    //Save settings before closing
    barcode_settings_manager.save_settings_to_file(barcode_settings_gui.get_current_barcode_settings());
    file_generation_settings_manager.save_settings_to_file(file_generation_settings_gui.get_current_file_generation_settings());

    delete file_generation_settings_layout;
    delete barcode_settings_layout;
    delete barcode_item;
    delete barcode_encoder;
    delete ui;
}

void MainWindow::add_settings_sections()
{
    barcode_settings_layout -> addWidget(&barcode_settings_gui);
    barcode_settings_expandable_section->set_content_layout(*barcode_settings_layout);
    main_layout.addWidget(barcode_settings_expandable_section);

    file_generation_settings_layout -> addWidget(&file_generation_settings_gui);
    file_generation_settings_expandable_section->set_content_layout(*file_generation_settings_layout);
    main_layout.addWidget(file_generation_settings_expandable_section);
}

void MainWindow::add_barcode_generation_buttons()
{
    fixed_part_of_the_barcode_button.setText("Add fixed barcode text");
    connect(&fixed_part_of_the_barcode_button, &QPushButton::clicked, this, &MainWindow::slot_add_fixed_barcode_part_gui);

    character_variable_part_of_the_barcode_button.setText("Add variable barcode character");
    connect(&character_variable_part_of_the_barcode_button, &QPushButton::clicked, this, &MainWindow::slot_add_character_variable_barcode_part_gui);

    number_variable_part_of_the_barcode_button.setText("Add variable barcode number");
    connect(&number_variable_part_of_the_barcode_button, &QPushButton::clicked, this, &MainWindow::slot_add_number_variable_barcode_part_gui);

    barcode_parts_buttons_layout.addWidget(&fixed_part_of_the_barcode_button);
    barcode_parts_buttons_layout.addWidget(&character_variable_part_of_the_barcode_button);
    barcode_parts_buttons_layout.addWidget(&number_variable_part_of_the_barcode_button);
}

void MainWindow::update_preview()
{
    barcode_item -> update();
    scene.update();

    if(silence_warnings_checkbox.isChecked())
        return;

    //Check if barcode with new settings fits selected page size
    if(barcode_item->boundingRect().width() > printer.pageRect(QPrinter::DevicePixel).width())
    {
        QMessageBox::warning(this, "Barcode size warning",
                             "Barcode is wider than selected page width.\n"
                             "Lower the value of barcode's width scale or set higher page width", QMessageBox::Ok | QMessageBox::NoButton);
        return;
    }

    int barcode_item_height = barcode_settings_gui.get_current_barcode_settings().barcode_height_in_mm;
    int page_height = file_generation_settings_gui.get_current_file_generation_settings().page_height_in_mm;
    int rows_per_page = file_generation_settings_gui.get_current_file_generation_settings().rows_per_page;

    int barcodes_column_height = rows_per_page*barcode_item_height;

    if(barcodes_column_height > page_height)
    {
        QMessageBox::warning(this, "Barcode size warning",
                             "Rows of barcodes will not fit into page with specified height.\n"
                             "Lower the value of barcode's height/rows per page or set higher page height" , QMessageBox::Ok | QMessageBox::NoButton);
        return;
    }
}

void MainWindow::add_barcode_preview()
{
    QString barcode_type_text = "";

    if(barcode_settings_manager.get_barcode_settings().barcode_type == BarcodeType::Code128)
    {
        barcode_item = new Code128BarcodeItem();
        barcode_type_text = "Code 128";
    }
    else
    {
        //... check for other barcode types implemented
    }

    barcode_item -> set_screen_resolution(printer.resolution());
    barcode_item -> setPos(0,0);
    barcode_item -> prepare_barcode_with_new_text(barcode_type_text, barcode_encoder);

    scene.addItem(barcode_item);
    scene.update();
    update_preview();

    connect(&barcode_settings_gui, &BarcodeSettingsGUI::signal_settings_updated, this, &MainWindow::slot_update_barcode_settings);
}

void MainWindow::add_actions_to_menu()
{
    QMenu *file_menu = menuBar() -> addMenu(tr("&File"));
    file_menu -> setTitle("File");

    QAction *generate_file_from_parts_action = new QAction(tr("&Generate file from barcode parts"), this);
    generate_file_from_parts_action -> setStatusTip(tr("Generating file from provided barcode parts"));
    connect(generate_file_from_parts_action, &QAction::triggered, this, &MainWindow::slot_generate_file_from_parts);
    file_menu -> addAction(generate_file_from_parts_action);

    QAction *generate_file_from_excel_action = new QAction(tr("&Generate barcodes file from *.csv file"), this);
    generate_file_from_excel_action -> setStatusTip(tr("Generating barcodes file from chosen *.csv file"));
    connect(generate_file_from_excel_action, &QAction::triggered, this, &MainWindow::slot_generate_file_from_csv);
    file_menu -> addAction(generate_file_from_excel_action);

    file_menu -> show();
}

void MainWindow::slot_update_barcode_text(QString new_barcode_text_part)
{
    QWidget *sender_widget = dynamic_cast<QWidget*>(QObject::sender());
    if(sender_widget == nullptr) //It should not happen...
        return;

    QWidget *parent_of_sender = dynamic_cast<QWidget*>(sender_widget -> parent());
    if(parent_of_sender == nullptr) //Again, it should not happen
        return;

    int widget_index = barcode_parts_layout.indexOf(parent_of_sender);
    if(widget_index == -1) //Widget not found
        return;

    if(barcode_text_parts.length() < widget_index + 1) //If text part for this QLineEdit was not added yet, add it
    {
        barcode_text_parts.append(new_barcode_text_part);
    }
    else //If text part already exists, replace it
        barcode_text_parts.replace(widget_index, new_barcode_text_part);

    current_barcode_text = barcode_text_parts.join("");
    barcode_item -> prepare_barcode_with_new_text(current_barcode_text, barcode_encoder);
    update_preview();
}

void MainWindow::slot_update_barcode_settings(BarcodeSettings new_barcode_settings)
{
    if(new_barcode_settings.barcode_type != currently_used_barcode_type)
    {
        BarcodeEncoder *barcode_encoder_to_delete = barcode_encoder;
        BarcodeItem *barcode_item_to_delete = barcode_item;

        if(new_barcode_settings.barcode_type == BarcodeType::Code128)
        {
            barcode_encoder = new Code128Encoder();
            barcode_item = new Code128BarcodeItem(new_barcode_settings);
        }
        else
        {
            //... check for other barcode types implemented
            //For now it stays here to silence the warning about using barcode_item after it is freed
            barcode_item = new Code128BarcodeItem(new_barcode_settings);
        }

        delete barcode_encoder_to_delete;
        delete barcode_item_to_delete;
    }

    barcode_item -> set_barcode_settings(new_barcode_settings);
    update_preview();
}

void MainWindow::generate_barcodes_for_fixed_barcode_part(FixedBarcodePartGUI *fixed_barcode_part_gui, QList<QString> *list_of_barcode_texts)
{
    QString barcode_text_part = fixed_barcode_part_gui -> get_line_edit() -> text();
    if(list_of_barcode_texts->length() == 0)
    {
        list_of_barcode_texts->append(barcode_text_part);
    }
    else
    {
        for(int i = 0; i < list_of_barcode_texts->length(); i++)
        {
            QString new_barcode_text = list_of_barcode_texts->at(i) + barcode_text_part;
            list_of_barcode_texts->replace(i, new_barcode_text);
        }
    }
}

void MainWindow::generate_barcodes_for_character_variable_barcode_part(CharacterVariableBarcodePartGUI *character_variable_barcode_part_gui, QList<QString> *list_of_barcode_texts)
{
    QString first_barcode_text_part = character_variable_barcode_part_gui -> get_line_edit() -> text();
    char first_barcode_text_part_as_char = first_barcode_text_part.at(0).toLatin1();
    int first_barcode_text_part_ASCII_index = (int)first_barcode_text_part_as_char;
    int increment_value = character_variable_barcode_part_gui -> get_increment_value();
    int steps_value = character_variable_barcode_part_gui -> get_steps_value();

    if(increment_value == 0 || steps_value == 0)
    {
        QMessageBox::warning(this, "Generation warning", "Increment value or steps value in barcode variable part not set" , QMessageBox::Ok | QMessageBox::NoButton);
    }

    if(list_of_barcode_texts->length() == 0)
    {
        for(int i = 0; i < steps_value + 1; i++)
        {
            int next_character_ASCII_index = first_barcode_text_part_ASCII_index + increment_value * i;
            char next_character = (char)next_character_ASCII_index;
            QString next_character_as_string = QString(QChar::fromLatin1(next_character));
            list_of_barcode_texts->append(next_character_as_string);
        }
    }
    else
    {
        QList<QString> *temp_list = new QList<QString>;
        int current_length_of_list_of_barcode_texts = list_of_barcode_texts->length();
        for(int i = 0; i < current_length_of_list_of_barcode_texts; i++)
        {
            QString temp_barcode_text = list_of_barcode_texts -> at(i);
            for(int j = 0; j < steps_value + 1; j++)
            {
                int next_character_ASCII_index = first_barcode_text_part_ASCII_index + increment_value * j;
                char next_character = (char)next_character_ASCII_index;
                QString next_character_as_string = QString(QChar::fromLatin1(next_character));
                QString new_barcode_text = temp_barcode_text + next_character_as_string;
                temp_list->append(new_barcode_text);
            }

        }

        list_of_barcode_texts->clear();
        for(int k = 0; k < temp_list->length(); k++)
        {
            list_of_barcode_texts->append(temp_list -> at(k));
        }

        temp_list -> clear();
        delete temp_list;
    }
}

void MainWindow::generate_barcodes_for_number_variable_barcode_part(NumberVariableBarcodePartGUI *number_variable_barcode_part_gui, QList<QString> *list_of_barcode_texts)
{
    QString first_barcode_text_part = number_variable_barcode_part_gui -> get_line_edit() -> text();
    int first_barcode_text_part_as_int = first_barcode_text_part.toInt();
    int increment_value = number_variable_barcode_part_gui -> get_increment_value();
    int steps_value = number_variable_barcode_part_gui -> get_steps_value();

    if(increment_value == 0 || steps_value == 0)
    {
        QMessageBox::warning(this, "Generation warning", "Increment value or steps value in barcode variable part not set" , QMessageBox::Ok | QMessageBox::NoButton);
    }

    if(list_of_barcode_texts->length() == 0)
    {
        for(int i = 0; i < steps_value + 1; i++)
        {
            int next_number_as_int = first_barcode_text_part_as_int + increment_value * i;
            QString next_number_as_string = QString::number(next_number_as_int);
            list_of_barcode_texts->append(next_number_as_string);
        }
    }
    else
    {
        QList<QString> *temp_list = new QList<QString>;

        int current_length_of_list_of_barcode_texts = list_of_barcode_texts->length();
        for(int i = 0; i < current_length_of_list_of_barcode_texts; i++)
        {
            QString temp_barcode_text = list_of_barcode_texts -> at(i);
            for(int j = 0; j < steps_value + 1; j++)
            {
                int next_number_as_int = first_barcode_text_part_as_int + increment_value * j;
                QString next_number_as_string = QString::number(next_number_as_int);
                QString new_barcode_text = temp_barcode_text + next_number_as_string;
                temp_list -> append(new_barcode_text);
            }
        }

        list_of_barcode_texts->clear();
        for(int k = 0; k < temp_list->length(); k++)
        {
            list_of_barcode_texts->append(temp_list -> at(k));
        }

        temp_list -> clear();
        delete temp_list;
    }
}

QList<QString>* MainWindow::generate_barcodes_list_from_parts()
{
    QList<QString> *list_of_barcode_texts = new QList<QString>;
    for(int i = 0; i < barcode_parts_layout.count(); i++)
    {
        FixedBarcodePartGUI *fixed_barcode_part_gui = dynamic_cast<FixedBarcodePartGUI*>(barcode_parts_layout.itemAt(i)->widget());
        if(fixed_barcode_part_gui != nullptr)
        {
            generate_barcodes_for_fixed_barcode_part(fixed_barcode_part_gui, list_of_barcode_texts);
            continue;
        }

        auto *character_variable_barcode_part_gui = dynamic_cast<CharacterVariableBarcodePartGUI*>(barcode_parts_layout.itemAt(i)->widget());
        if(character_variable_barcode_part_gui != nullptr)
        {
            generate_barcodes_for_character_variable_barcode_part(character_variable_barcode_part_gui, list_of_barcode_texts);
            continue;
        }

        auto *number_variable_barcode_part_gui = dynamic_cast<NumberVariableBarcodePartGUI*>(barcode_parts_layout.itemAt(i)->widget());
        if(number_variable_barcode_part_gui != nullptr)
        {
            generate_barcodes_for_number_variable_barcode_part(number_variable_barcode_part_gui, list_of_barcode_texts);
            continue;
        }
    }

    return list_of_barcode_texts;
}

QList<QString> *MainWindow::load_barcodes_list_from_csv()
{
    QList<QString> *list_of_barcode_texts = new QList<QString>;

    QString file_name = get_file_with_data_to_generate_as_barcodes();
    QFile csv_file_with_barcodes_data(file_name);

    if(!csv_file_with_barcodes_data.open(QIODevice::ReadOnly))
    {
        QString warning_message = "File " + file_name + " could not be opened";
        QMessageBox::warning(this, "File loading warning", warning_message, QMessageBox::Ok | QMessageBox::NoButton);
        csv_file_with_barcodes_data.close();
        return list_of_barcode_texts;
    }

    QTextStream csv_file_text_stream(&csv_file_with_barcodes_data);
    QString first_row = csv_file_text_stream.readLine();

    char separator = ',';
    if (!first_row.contains(','))
        separator = '\t';

    while (!csv_file_text_stream.atEnd())
    {
       QList<QString> temp_list = csv_file_text_stream.readLine().split(separator, Qt::SkipEmptyParts);
       list_of_barcode_texts->append(temp_list);
    }

    return list_of_barcode_texts;
}

QString MainWindow::get_file_for_barcodes_generation()
{
    QString file_path;
    QStringList formats_list = {"*.pdf"};

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDirectory(qApp->applicationDirPath());
    dialog.setNameFilters(formats_list);
    dialog.setWindowTitle("Barcodes file saving");

    QStringList file_paths = QStringList();

    if(dialog.exec())
        file_paths = dialog.selectedFiles();

    if(file_paths.size() > 0)
        file_path = file_paths.at(0);

    return file_path;
}

QString MainWindow::get_file_with_data_to_generate_as_barcodes()
{
    QString file_path;
    QStringList formats_list = {"*.csv"};

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDirectory(qApp->applicationDirPath());
    dialog.setNameFilters(formats_list);
    dialog.setWindowTitle("Selecting file with barcode data to be generated");

    QStringList file_paths = QStringList();

    if(dialog.exec())
        file_paths = dialog.selectedFiles();

    if(file_paths.size() > 0)
        file_path = file_paths.at(0);

    return file_path;
}

void MainWindow::prepare_printer_settings(FileGenerationSettings &file_generation_settings)
{
    QPageSize page_size = QPageSize(QSizeF(file_generation_settings.page_width_in_mm, file_generation_settings.page_height_in_mm),
                                    QPageSize::Unit::Millimeter, NULL, QPageSize::SizeMatchPolicy::ExactMatch);
    printer.setPageSize(page_size);
    printer.setOutputFileName(file_name);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setFullPage(true);
    printer.setPageMargins(QMarginsF(0,0,0,0));
    barcodes_document.setPageSize(QSizeF(printer.pageRect().size()));
    barcodes_document.setDocumentMargin(0);

    if(file_generation_settings.page_orientation == Qt::Orientation::Horizontal)
        printer.setPageOrientation(QPageLayout::Orientation::Portrait);
    else
        printer.setPageOrientation(QPageLayout::Orientation::Portrait);
    //Landscape for now not supported. Something with squishing images.
}

bool MainWindow::check_if_file_can_be_generated(int spacer_height, QList<QString> *list_of_barcode_texts)
{
    if(spacer_height < 0)
    {
        QMessageBox::warning(this, "Generation warning",
                             "Resulting horizontal spacer for even barcodes distribution is negative.\n"
                             "Lower the number of rows per page or barcodes width scale" , QMessageBox::Ok | QMessageBox::NoButton);
        return false;
    }

    //Barcodes can get wider if number variables were used.
    //It might not work properly for file generation using data loaded from unsorted csv file!
    barcode_item->prepare_barcode_with_new_text(list_of_barcode_texts->last(), barcode_encoder);
    barcode_item->update();

    if(barcode_item->boundingRect().width() >= printer.pageRect(QPrinter::DevicePixel).width())
    {
        QMessageBox::warning(this, "Generation warning",
                             "Barcode is wider than selected page width.\n"
                             "Lower the value of barcode's width scale or set higher page width.\n"
                             "It is possible that this happened because number variable added new character(s) to barcodes text",
                             QMessageBox::Ok | QMessageBox::NoButton);
        return false;
    }
    return true;
}

void MainWindow::insert_barcode_into_document(FileGenerationSettings current_file_generation_settings, int &rows_counter, int i, double horizontal_spacer_height_in_device_pixels, QTextCursor &cursor, QList<QString> *list_of_barcode_texts)
{
    if(rows_counter == 0)
    {
        QTextBlockFormat horizontal_spacer_block;
        horizontal_spacer_block.setAlignment(Qt::AlignHCenter);
        horizontal_spacer_block.setLineHeight(horizontal_spacer_height_in_device_pixels/2, QTextBlockFormat::FixedHeight);
        cursor.insertBlock(horizontal_spacer_block);
    }
    rows_counter++;

    barcode_item -> prepare_barcode_with_new_text(list_of_barcode_texts->at(i), barcode_encoder);
    QRectF barcode_bounding_rect = barcode_item ->boundingRect();
    QPixmap pix(barcode_bounding_rect.width(), barcode_bounding_rect.height());
    QPainter paint(&pix);
    pix.fill(Qt::white);
    barcode_item -> paint(&paint, nullptr, nullptr);
    QImage barcode_image = pix.toImage();

    QTextBlockFormat alignment_format;
    alignment_format.setAlignment(Qt::AlignHCenter);

    if(rows_counter == current_file_generation_settings.rows_per_page)
        alignment_format.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter);

    cursor.insertBlock(alignment_format);
    cursor.insertImage(barcode_image);

    if(rows_counter == current_file_generation_settings.rows_per_page)
    {
        rows_counter = 0;
        return;
    }

    QTextBlockFormat horizontal_spacer_block;
    horizontal_spacer_block.setAlignment(Qt::AlignHCenter);
    horizontal_spacer_block.setLineHeight(horizontal_spacer_height_in_device_pixels, QTextBlockFormat::FixedHeight);
    cursor.insertBlock(horizontal_spacer_block);
}

void MainWindow::generate_barcodes_file(QList<QString> *list_of_barcode_texts)
{
    FileGenerationSettings current_file_generation_settings = file_generation_settings_gui.get_current_file_generation_settings();
    prepare_printer_settings(current_file_generation_settings);

    //Get horizontal spacers for even distribution on the page
    //To give each barcode equal blank spaces on both sides (to enable, for example, cutting the paper into equal sized barcodes)
    //insert full sized horizontal spacer between barcodes and halves on top and bottom of the page.
    int horizontal_spacers_count = current_file_generation_settings.rows_per_page;

    double page_height_in_device_pixels = printer.pageRect(QPrinter::DevicePixel).height();
    double barcode_height = barcode_item -> boundingRect().height();
    double barcodes_column_height = barcode_height*current_file_generation_settings.rows_per_page;
    double horizontal_spacer_height_in_device_pixels = double((page_height_in_device_pixels - barcodes_column_height)/horizontal_spacers_count);

    bool file_can_be_generated = check_if_file_can_be_generated(horizontal_spacer_height_in_device_pixels, list_of_barcode_texts);
    if(!file_can_be_generated)
        return;

    int rows_counter = 0;
    QTextCursor cursor(&barcodes_document);

    file_generation_progress_dialog->setLabelText("Generating barcodes...");
    for(int i = 0; i < list_of_barcode_texts->length(); i++)
    {
        insert_barcode_into_document(current_file_generation_settings, rows_counter, i, horizontal_spacer_height_in_device_pixels, cursor, list_of_barcode_texts);
        file_generation_progress_dialog->setValue(i);

        if(file_generation_progress_dialog->wasCanceled())
            break;
    }

    file_generation_progress_dialog->setLabelText("Printing barcodes to file...");
    file_generation_progress_dialog->update();

    //Deleting first empty line, that I do not know why is added.
    cursor.setPosition(0);
    cursor.deleteChar();

    barcodes_document.print(&printer);
    barcodes_document.clear();
}

//Somehow reduce the number of slots generating files? Ideally to one.
//Option - Pass a pointer to the function which is supposed to create list of barcodes for the specific case, use lambda?
//To take into consideration if more options for file generation were to be added.
void MainWindow::slot_generate_file_from_parts()
{
    //Prepare file for generating barcodes
    file_name = get_file_for_barcodes_generation();

    if(file_name.isEmpty())
        return;

    QFile barcodes_file(file_name);
    barcodes_file.resize(0);

    //Prepare barcodes texts to encode and progress dialog to show the user that it is still working
    file_generation_progress_dialog = new QProgressDialog("Assesing task's time...", "Abort generation", 0, 1, this);
    file_generation_progress_dialog->setWindowModality(Qt::WindowModal);
    QList<QString> *list_of_barcode_texts = generate_barcodes_list_from_parts();

    if(list_of_barcode_texts->isEmpty())
    {
        list_of_barcode_texts->clear();
        delete list_of_barcode_texts;

        file_generation_progress_dialog->deleteLater();
        file_generation_progress_dialog = nullptr;

        QMessageBox::warning(this, "Generation warning",
                             "No barcodes to be generated were found.", QMessageBox::Ok | QMessageBox::NoButton);
        return;
    }

    file_generation_progress_dialog->setMaximum(list_of_barcode_texts->length());
    generate_barcodes_file(list_of_barcode_texts);

    file_generation_progress_dialog->close();
    delete file_generation_progress_dialog;
    file_generation_progress_dialog = nullptr;

    list_of_barcode_texts->clear();
    delete list_of_barcode_texts;
}

void MainWindow::slot_generate_file_from_csv()
{
    //Prepare file for generating barcodes
    file_name = get_file_for_barcodes_generation();

    if(file_name.isEmpty())
        return;

    QFile barcodes_file(file_name);
    barcodes_file.resize(0);

    //Prepare barcodes texts to encode and progress dialog to show the user that it is still working
    file_generation_progress_dialog = new QProgressDialog("Assesing task's time...", "Abort generation", 0, 1, this);
    file_generation_progress_dialog->setWindowModality(Qt::WindowModal);
    QList<QString> *list_of_barcode_texts = load_barcodes_list_from_csv();

    if(list_of_barcode_texts->isEmpty())
    {
        list_of_barcode_texts->clear();
        delete list_of_barcode_texts;

        file_generation_progress_dialog->deleteLater();
        file_generation_progress_dialog = nullptr;

        QMessageBox::warning(this, "Generation warning",
                             "No barcodes to be generated were found.", QMessageBox::Ok | QMessageBox::NoButton);
        return;
    }

    file_generation_progress_dialog->setMaximum(list_of_barcode_texts->length());
    generate_barcodes_file(list_of_barcode_texts);

    file_generation_progress_dialog->close();
    delete file_generation_progress_dialog;
    file_generation_progress_dialog = nullptr;

    list_of_barcode_texts->clear();
    delete list_of_barcode_texts;
}

void MainWindow::slot_add_fixed_barcode_part_gui()
{
    FixedBarcodePartGUI *fixed_barcode_part_GUI = new FixedBarcodePartGUI(this);
    barcode_parts_layout.addWidget(fixed_barcode_part_GUI);

    connect(fixed_barcode_part_GUI -> get_line_edit(), &QLineEdit::textEdited, this, &MainWindow::slot_update_barcode_text);
    connect(fixed_barcode_part_GUI, &FixedBarcodePartGUI::signal_remove_barcode_part, this, &MainWindow::slot_remove_barcode_part);
}

void MainWindow::slot_add_character_variable_barcode_part_gui()
{
    CharacterVariableBarcodePartGUI *character_variable_barcode_part_GUI = new CharacterVariableBarcodePartGUI(this);
    barcode_parts_layout.addWidget(character_variable_barcode_part_GUI);

    connect(character_variable_barcode_part_GUI -> get_line_edit(), &QLineEdit::textEdited, this, &MainWindow::slot_update_barcode_text);
    connect(character_variable_barcode_part_GUI, &CharacterVariableBarcodePartGUI::signal_remove_barcode_part, this, &MainWindow::slot_remove_barcode_part);
}

void MainWindow::slot_add_number_variable_barcode_part_gui()
{
    NumberVariableBarcodePartGUI *number_variable_barcode_part_GUI = new NumberVariableBarcodePartGUI(this);
    barcode_parts_layout.addWidget(number_variable_barcode_part_GUI);

    connect(number_variable_barcode_part_GUI -> get_line_edit(), &QLineEdit::textEdited, this, &MainWindow::slot_update_barcode_text);
    connect(number_variable_barcode_part_GUI, &NumberVariableBarcodePartGUI::signal_remove_barcode_part, this, &MainWindow::slot_remove_barcode_part);
}

void MainWindow::slot_remove_barcode_part()
{
    QWidget *sender_widget = dynamic_cast<QWidget*>(QObject::sender());
    if(sender_widget == nullptr)
        return;

    int widget_index = barcode_parts_layout.indexOf(sender_widget);

    if(widget_index == -1)
        return;

    if(barcode_text_parts.length() > widget_index) //Text part for this widget was already added, must be removed
        barcode_text_parts.removeAt(widget_index);

    sender_widget -> deleteLater();

    current_barcode_text = barcode_text_parts.join("");
    barcode_item -> prepare_barcode_with_new_text(current_barcode_text, barcode_encoder);
    barcode_item -> update();
    scene.update();
}
