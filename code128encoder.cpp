#include "code128encoder.h"

QString Code128Encoder::symbols[108] = {
"212222", "222122", "222221", "121223", "121322", "131222", "122213", "122312", "132212", "221213", "221312", "231212", "112232",
"122132", "122231", "113222", "123122", "123221", "223211", "221132", "221231", "213212", "223112", "312131", "311222", "321122",
"321221", "312212", "322112", "322211", "212123", "212321", "232121", "111323", "131123", "131321", "112313", "132113", "132311",
"211313", "231113", "231311", "112133", "112331", "132131", "113123", "113321", "133121", "313121", "211331", "231131", "213113",
"213311", "213131", "311123", "311321", "331121", "312113", "312311", "332111", "314111", "221411", "431111", "111224", "111422",
"121124", "121421", "141122", "141221", "112214", "112412", "122114", "122411", "142112", "142211", "241211", "221114", "413111",
"241112", "134111", "111242", "121142", "121241", "114212", "124112", "124211", "411212", "421112", "421211", "212141", "214121",
"412121", "111143", "111341", "131141", "114113", "114311", "411113", "411311", "113141", "114131", "311141", "411131", "211412",
"211214", "211232", "2331112", "211133"};

Code128Encoder::Code128Encoder(){}

Code128Encoder::~Code128Encoder(){}

QString Code128Encoder::get_code128_symbol_for_ASCII_character(int characters_index_in_ASCII)
{
    if (characters_index_in_ASCII > Stop_pattern)
    {
        return symbols[107];
    }

    return symbols[characters_index_in_ASCII];
}

void Code128Encoder::add_char_as_symbol_to_barcode(char c, QStringList &barcode_symbols, int &checksum)
{
    int characters_index_in_ASCII = (int)c;

    //Check if characters are in valid range - if not, replace them with space (ASCII 32)
    if (characters_index_in_ASCII < 32)
    {
        characters_index_in_ASCII = 32;
    }
    if (characters_index_in_ASCII > 126)
    {
        characters_index_in_ASCII = 32;
    }

    int code128_symbol_index = characters_index_in_ASCII - 32;
    QString symbol = get_code128_symbol_for_ASCII_character(code128_symbol_index);
    barcode_symbols.append(symbol);

    checksum += code128_symbol_index * (barcode_symbols.count() == 1 ? 1 : barcode_symbols.count() - 1);
}

void Code128Encoder::add_int_as_symbol_to_barcode(int characters_index_in_ASCII, QStringList &barcode_symbols, int &checksum)
{
    QString symbol = get_code128_symbol_for_ASCII_character(characters_index_in_ASCII);
    barcode_symbols.append(symbol);
    checksum += characters_index_in_ASCII * (barcode_symbols.count() == 1 ? 1 : barcode_symbols.count() - 1);
}

quint8 Code128Encoder::count_digits(const QString &data, int start_pos)
{
    quint8 counter = 0;

    for ( ;start_pos < data.length(); start_pos++)
    {
        if (!data.at(start_pos).isDigit())
        {
            break;
        }
        counter++;
    }

    //Subcode C encodes pairs of digits
    counter &= 0xfe;

    return counter;
}

void Code128Encoder::encode_next_character(QStringList &barcode_symbols, Mode &mode, const QString &data, int &checksum, int &pos)
{
    quint8 digits_counted = count_digits(data, pos);

    if (digits_counted >= 4)
    {
        if (mode != MODE_C)
        {
            //Change mode to subcode C to save on barcode space if there are pairs of digits
            if (mode == IDLE)
            {
                add_int_as_symbol_to_barcode(Special_symbols::Start_C, barcode_symbols, checksum);
            }
            else
            {
                add_int_as_symbol_to_barcode(Special_symbols::Code_C, barcode_symbols, checksum);
            }
            mode = MODE_C;
        }

        int two_digit_numbers_counted = digits_counted>>1; //right shift, equivalent to digits_counted/2
        for (int i = 0; i < two_digit_numbers_counted; i++)
        {
            QString two_digit_number = data.mid(pos, 2);
            int two_digit_number_value = two_digit_number.toInt();
            add_int_as_symbol_to_barcode(two_digit_number_value, barcode_symbols, checksum);
            pos += 2;
        }
    }
    else
    {
        if ( mode != MODE_B )
        {
            if ( mode == IDLE )
            {
                add_int_as_symbol_to_barcode(Special_symbols::Start_B, barcode_symbols, checksum);
            }
            else
            {
                add_int_as_symbol_to_barcode(Special_symbols::Code_B, barcode_symbols, checksum);
            }
            mode = MODE_B;
        }
        add_char_as_symbol_to_barcode(data.at(pos).toLatin1(), barcode_symbols, checksum);
        pos++;
    }
}

QList<int> Code128Encoder::encode(const QString data)
{
    QStringList barcode_symbols;
    int checksum = 0;

    if (data.length() > 100 || data.length() == 0)
    {
        QList<int> dummy_list;
        return dummy_list;
    }

    int pos = 0;
    Mode mode = IDLE;

    while (pos < data.length())
    {
        encode_next_character(barcode_symbols, mode, data, checksum, pos);
    }

    quint8 remainder = checksum % 103;
    add_int_as_symbol_to_barcode(remainder, barcode_symbols, checksum);
    add_int_as_symbol_to_barcode(Special_symbols::Stop_pattern, barcode_symbols, checksum);

    //Get bar widths as integers from created list of symbols
    QList<int> bar_widths_list;
    QString barcode_symbols_joined = barcode_symbols.join("");

    for(int i = 0; i < barcode_symbols_joined.length(); i++)
    {
        QString single_bar_width = barcode_symbols_joined.at(i);
        bar_widths_list.append(single_bar_width.toInt());
    }

    return bar_widths_list;
}
