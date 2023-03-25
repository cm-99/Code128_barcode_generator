#ifndef CODE128ENCODER_H
#define CODE128ENCODER_H

#include "barcodeencoder.h"

#include <QStringList>

/**
 * Implementation of BarcodeEncoder for Code128 barcode.
 */
class Code128Encoder: public BarcodeEncoder
{
public:
    Code128Encoder();
    ~Code128Encoder();
    /**
     * Code 128 symbols representation. 108 symbols include: 103 data symbols, 3 start symbols and 2 stop symbols.
     * Each symbol consists of three black bars and three white spaces of varying widths -
     * from 1 to 4 "modules".
     * The odd numbered bars are dark and the even numbered bars are light.
     *
     * [Deviation from standard symbols]
     * In place of a stop symbol ("233111") there is a stop pattern ("2331112").
     * Stop pattern is a stop symbol + quiet zone (2 modules wide space after barcode).
    */
    static QString symbols[108];

    /**
     * Encodes given @param: data to Code128.
     * Accepts subset of ASCII printable characters from 32 to 126 (Code 128B)
     * Maximum data lenght is 100 characters.
     * @return - returns a list of bar widths.
     */
    QList<int> encode(const QString data) override;

private:
    /**
     * Special symbols.
     * e.g. Start_A (Start Code A), Start_B and Start_C cause all following symbols to be interpreted according to the corresponding subcode.
     */
    enum Special_symbols {Code_C = 99, Code_B = 100, Start_A = 103, Start_B = 104, Start_C = 105, Stop_pattern = 106};
    /**
     * Encoding modes.
     */
    enum Mode {MODE_A, MODE_B, MODE_C, IDLE};

    static quint8 count_digits(const QString &data, int start_pos);
    static QString get_code128_symbol_for_ASCII_character(int characters_index_in_ASCII);
    static void add_char_as_symbol_to_barcode(char c, QStringList &barcode_symbols, int &checksum);
    static void add_int_as_symbol_to_barcode(int characters_index_in_ASCII, QStringList &barcode_symbols, int &checksum);
    /**
     * Encodes next character(s) from @param:data starting at position @param:pos.
     * Appends encoded symbols to @param:barcode_symbols.
     *
     * Method has been extracted for better code readability but the name encode_next_character is a bit of a lie.
     * It is true only if encoding mode stays the same and there are no pairs of digits in @param:data.
     *
     * If subsequent digits are found in @param:data using count_digits, then more than one symbol can be appended
     * to @param:barcode_symbols at a time (to be exact, result of count_digits/2 symbols will be appended, using subcode C).
     * That is also excluding special symbols encoding for subcode changes.
     */
    static void encode_next_character(QStringList &barcode_symbols, Mode &mode, const QString &data, int &checksum, int &pos);
};

#endif // CODE128ENCODER_H
