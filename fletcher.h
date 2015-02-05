#include "bktypes.h"

#define INITIAL32 0x00000000

Long fletcher32Byte(Byte b, Long checksum);
Long fletcher32(Byte *data, Long len);
Short fletcher16(Byte *data, Long len);
