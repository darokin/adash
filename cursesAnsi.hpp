#ifndef ADASH_CURSESANSI_H
#define ADASH_CURSESANSI_H

#include <fstream>

#define ASCII_ESC           '\x1B'
#define ASCII_EOF           '\x1A'
#define ASCII_BELL          '\x07'
#define ASCII_BACKSPACE     '\x08'
#define ASCII_HORIZONTALTAB '\x09'
#define ASCII_LINEFEED      '\x0A'
#define ASCII_VERTICALTAB   '\x0B'
#define ASCII_FORMFEED      '\x0C'
#define ASCII_CARRIAGERETURN '\x0D'
#define ASCII_DELETE        '\x7F'

#define MOD_START            '['
#define MOD_COMPLEX_2_UNDO   '2'
#define MOD_COMPLEX_3_FG     '3'
#define MOD_COMPLEX_4_BG     '4'

#define MOD_0_RESET     '0'
#define MOD_1_BOLD      '1'
#define MOD_2_DIM       '2'
#define MOD_3_ITALIC    '3'
#define MOD_4_UNDERLINE '4'
#define MOD_5_BLINK     '5'

#define MOD_7_REVERSE   '7'
#define MOD_8_HIDDEN    '8'
#define MOD_9_STRIKE    '9'

#define MOD_END         'm'
#define MOD_SPLITTER    ';'

#define ASCII_DIGIT_START   48

#define ANSI_COLORSTART_FG  30
#define ANSI_COLORSTART_BG  40

#define ANSI_COLOR_BLACK	0
#define ANSI_COLOR_RED		1
#define ANSI_COLOR_GREEN	2
#define ANSI_COLOR_YELLOW	3
#define ANSI_COLOR_BLUE		4
#define ANSI_COLOR_MAGENTA	5
#define ANSI_COLOR_CYAN		6
#define ANSI_COLOR_WHITE	7
#define ANSI_COLOR_COUNT	8

namespace cursesANSI {
	static const wchar_t* CP437_unicode[128] = { \
		L"\u00C7", // Ç		0		128
		L"\u00FC", // ü		1		129
		L"\u00E9", // é		2		130
		L"\u00E2", // â		3		131
		L"\u00E4", // ä		4		132
		L"\u00E0", // à		5		133
		L"\u00E5", // å		6		134
		L"\u00E7", // ç		7		135
		L"\u00EA", // ê		8		136
		L"\u00EB", // ë		9		137
		L"\u00E8", // è		10		138
		L"\u00EF", // ï		11		139
		L"\u00EE", // î		12		140
		L"\u00EC", // ì		13		141
		L"\u00C4", // Ä		14		142
		L"\u00C5", // Å		15		143
		L"\u00C9", // É		16		144
		L"\u00E6", // æ		17		145
		L"\u00C6", // Æ		18		146
		L"\u00F4", // ô		19		147
		L"\u00F6", // ö		20		148
		L"\u00F2", // ò		21		149
		L"\u00FB", // û		22		150
		L"\u00F9", // ù		23		151
		L"\u00FF", // ÿ		24		152
		L"\u00D6", // Ö		25		153
		L"\u00DC", // Ü		26		154
		L"\u00A2", // ¢		27		155
		L"\u00A3", // £		28		156
		L"\u00A5", // ¥		29		157
		L"\u20A7", // ₧		30		158
		L"\u0192", // ƒ		31		169
		L"\u00E1", // á		32		160
		L"\u00ED", // í		33		161
		L"\u00F3", // ó		34		162
		L"\u00FA", // ú		35		163
		L"\u00F1", // ñ		36		164
		L"\u00D1", // Ñ		37		165
		L"\u00AA", // ª		38		166
		L"\u00BA", // º		39		167
		L"\u00BF", // ¿		40		168
		L"\u2310", // ⌐		41		179
		L"\u00AC", // ¬		42		170
		L"\u00BD", // ½		43		171
		L"\u00BC", // ¼		44		172
		L"\u00A1", // ¡		45		173
		L"\u00AB", // «		46		174
		L"\u00BB", // »		47		175
		L"\u2591", // ░		48		176
		L"\u2592", // ▒		49		177
		L"\u2593", // ▓		50		178
		L"\u2502", // │		51		179
		L"\u2524", // ┤		52		180
		L"\u2561", // ╡		53		181
		L"\u2562", // ╢		54		182
		L"\u2556", // ╖		55		183
		L"\u2555", // ╕		56		184
		L"\u2563", // ╣		57		185
		L"\u2551", // ║		58		186
		L"\u2557", // ╗		59		187
		L"\u255D", // ╝		60		188
		L"\u255C", // ╜		61		189
		L"\u255B", // ╛		62		190
		L"\u2510", // ┐		63		191
		L"\u2514", // └		64		192
		L"\u2534", // ┴		65		193
		L"\u252C", // ┬		66		194
		L"\u251C", // ├		67		195
		L"\u2500", // ─		68		196
		L"\u253C", // ┼		69		197
		L"\u255E", // ╞		70		198
		L"\u255F", // ╟		71		199
		L"\u255A", // ╚		72		200
		L"\u2554", // ╔		73		201
		L"\u2569", // ╩		74		202
		L"\u2566", // ╦		75		203
		L"\u2560", // ╠		76		204
		L"\u2550", // ═		77		205
		L"\u256C", // ╬		78		206
		L"\u2567", // ╧		79		207
		L"\u2568", // ╨		80		208
		L"\u2564", // ╤		81		209
		L"\u2565", // ╥		82		210
		L"\u2559", // ╙		83		211
		L"\u2558", // ╘		84		212
		L"\u2552", // ╒		85		213
		L"\u2553", // ╓		86		214
		L"\u256B", // ╫		87		215
		L"\u256A", // ╪		88		216
		L"\u2518", // ┘		89		217
		L"\u250C", // ┌		90		218
		L"\u2588", // █		91		219
		L"\u2584", // ▄		92		220
		L"\u258C", // ▌		93		221
		L"\u2590", // ▐		94		222
		L"\u2580", // ▀		95		223
		L"\u03B1", // α		96		224
		L"\u00DF", // ß		97		225
		L"\u0393", // Γ		98		226
		L"\u03C0", // π		99		227
		L"\u03A3", // Σ		100		228
		L"\u03C3", // σ		101		229
		L"\u00B5", // µ		102		230
		L"\u03C4", // τ		103		231
		L"\u03A6", // Φ		104		232
		L"\u0398", // Θ		105		233
		L"\u03A9", // Ω		106		234
		L"\u03B4", // δ		107		235
		L"\u221E", // ∞		108		236
		L"\u03C6", // φ		109		237
		L"\u03B5", // ε		110		238
		L"\u2229", // ∩		111		239
		L"\u2261", // ≡		112		240
		L"\u00B1", // ±		113		241
		L"\u2265", // ≥		114		242
		L"\u2264", // ≤		115		243
		L"\u2320", // ⌠		116		244
		L"\u2321", // ⌡		117		245
		L"\u00F7", // ÷		118		246
		L"\u2248", // ≈		119		247
		L"\u00B0", // °		120		248
		L"\u2219", // ∙		121		249
		L"\u00B7", // ·		122		250
		L"\u221A", // √		123		251
		L"\u207F", // ⁿ		124		252
		L"\u00B2", // ²		125		253
		L"\u25A0", // ■		126		254
		L"\u00A0" // NBSP	127		255
	};

    bool mvwaddnANSIstr(WINDOW* _win, int _startY, int _startX, std::string _line, size_t _size);
    unsigned char feedChar(unsigned char c, WINDOW* _win);

	
}; // end namespace

#endif