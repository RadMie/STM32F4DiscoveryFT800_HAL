/**************************************************************************
 *
 *                  FTDIChip AN_xxx FT800 with ARM - Version 1.0
 *
 **************************************************************************
 * FileName:        FT800.h
 * Purpose:         This header file contains the display list and graphics 
 *                  processor commands for the FTDI FT800 graphics controller chip
 * Dependencies:    See INCLUDES section below
 * Company:         Future Technology Devices International Ltd.
 *
 * Software License Agreement
 *
 * This code is provided as an example only and is not guaranteed by FTDI. 
 * FTDI accept no responsibility for any issues resulting from its use. 
 * The developer of the final application incorporating any parts of this 
 * sample project is responsible for ensuring its safe and correct operation 
 * and for any conequences resulting from its use.
 *
 * Author	Date				Rev		Comment
 **************************************************************************
 * BR			2014-03-01	1.0		Initial version - adapted from AN_275 source
 **************************************************************************

*/

#ifndef __ft_gpu_h
#define __ft_gpu_h
   
/*****************************************************************************/
/************************* I N C L U D E S ***********************************/
/*****************************************************************************/

/*****************************************************************************/
/********************** D E C L A R A T I O N S ******************************/
/*****************************************************************************/

#define FT_DL_SIZE            (8*1024)  //8KB Display List buffer size
#define FT_CMD_FIFO_SIZE      (4*1024)  //4KB coprocessor Fifo size
#define FT_CMD_SIZE           (4)       //4 byte per coprocessor command of EVE

#define FT800_VERSION         "1.9.0"

// FT800 Chip Commands - use with cmdWrite
#define FT800_ACTIVE					0x00			// Initializes FT800
#define FT800_STANDBY					0x41			// Place FT800 in Standby (clk running)
#define FT800_SLEEP						0x42			// Place FT800 in Sleep (clk off)
#define FT800_PWRDOWN					0x50			// Place FT800 in Power Down (core off)
#define FT800_CLKEXT					0x44			// Select external clock source
#define FT800_CLK48M					0x62			// Select 48MHz PLL
#define FT800_CLK36M					0x61			// Select 36MHz PLL
#define FT800_CORERST					0x68			// Reset core - all registers default

// FT800 Memory Commands - use with ft800memWritexx and ft800memReadxx
#define MEM_WRITE							0x80			// FT800 Host Memory Write 
#define MEM_READ							0x00			// FT800 Host Memory Read


// Refer to the FT800 Datasheet

// FT800 Memory Map Addresses
#define RAM_CMD               0x108000UL
#define RAM_DL                0x100000UL
#define RAM_G                 0x000000UL
#define RAM_PAL               0x102000UL
#define RAM_REG               0x102400UL

// FT800 Register Addresses
#define REG_CLOCK             0x102408UL
#define REG_CMD_DL            0x1024ecUL
#define REG_CMD_READ          0x1024e4UL
#define REG_CMD_WRITE         0x1024e8UL
#define REG_CPURESET          0x10241cUL
#define REG_CSPREAD           0x102464UL
#define REG_DITHER            0x10245cUL
#define REG_DLSWAP            0x102450UL
#define REG_FRAMES            0x102404UL
#define REG_FREQUENCY         0x10240cUL
#define REG_GPIO              0x102490UL
#define REG_GPIO_DIR          0x10248cUL
#define REG_HCYCLE            0x102428UL
#define REG_HOFFSET           0x10242cUL
#define REG_HSIZE             0x102430UL
#define REG_HSYNC0            0x102434UL
#define REG_HSYNC1            0x102438UL
#define REG_ID                0x102400UL
#define REG_INT_EN            0x10249cUL
#define REG_INT_FLAGS         0x102498UL
#define REG_INT_MASK          0x1024a0UL
#define REG_MACRO_0           0x1024c8UL
#define REG_MACRO_1           0x1024ccUL
#define REG_OUTBITS           0x102458UL
#define REG_PCLK              0x10246cUL
#define REG_PCLK_POL          0x102468UL
#define REG_PLAY              0x102488UL
#define REG_PLAYBACK_FORMAT   0x1024b4UL
#define REG_PLAYBACK_FREQ     0x1024b0UL
#define REG_PLAYBACK_LENGTH   0x1024a8UL
#define REG_PLAYBACK_LOOP     0x1024b8UL
#define REG_PLAYBACK_PLAY     0x1024bcUL
#define REG_PLAYBACK_READPTR  0x1024acUL
#define REG_PLAYBACK_START    0x1024a4UL
#define REG_PWM_DUTY          0x1024c4UL
#define REG_PWM_HZ            0x1024c0UL
#define REG_RENDERMODE        0x102410UL
#define REG_ROTATE            0x102454UL
#define REG_SNAPSHOT          0x102418UL
#define REG_SNAPY             0x102414UL
#define REG_SOUND             0x102484UL
#define REG_SWIZZLE           0x102460UL
#define REG_TAG               0x102478UL
#define REG_TAG_X             0x102470UL
#define REG_TAG_Y             0x102474UL
#define REG_TAP_CRC           0x102420UL
#define REG_TAP_MASK          0x102424UL
#define REG_TOUCH_ADC_MODE    0x1024f4UL
#define REG_TOUCH_CHARGE      0x1024f8UL
#define REG_TOUCH_DIRECT_XY   0x102574UL
#define REG_TOUCH_DIRECT_Z1Z2 0x102578UL
#define REG_TOUCH_MODE        0x1024f0UL
#define REG_TOUCH_OVERSAMPLE  0x102500UL
#define REG_TOUCH_RAW_XY      0x102508UL
#define REG_TOUCH_RZ          0x10250cUL
#define REG_TOUCH_RZTHRESH    0x102504UL
#define REG_TOUCH_SCREEN_XY   0x102510UL
#define REG_TOUCH_SETTLE      0x1024fcUL
#define REG_TOUCH_TAG         0x102518UL
#define REG_TOUCH_TAG_XY      0x102514UL
#define REG_TOUCH_TRANSFORM_A 0x10251cUL
#define REG_TOUCH_TRANSFORM_B 0x102520UL
#define REG_TOUCH_TRANSFORM_C 0x102524UL
#define REG_TOUCH_TRANSFORM_D 0x102528UL
#define REG_TOUCH_TRANSFORM_E 0x10252cUL
#define REG_TOUCH_TRANSFORM_F 0x102530UL
#define REG_TRACKER           0x109000UL
#define REG_VCYCLE            0x10243cUL
#define REG_VOFFSET           0x102440UL
#define REG_VOL_PB            0x10247cUL
#define REG_VOL_SOUND         0x102480UL
#define REG_VSIZE             0x102444UL
#define REG_VSYNC0            0x102448UL
#define REG_VSYNC1            0x10244cUL

// Graphics Engine Commands
// Refer to the FT800 Programmers Guide
#define CMDBUF_SIZE           4096UL
#define CMD_APPEND            0xffffff1eUL
#define CMD_BGCOLOR           0xffffff09UL
#define CMD_BUTTON            0xffffff0dUL
#define CMD_CALIBRATE         0xffffff15UL
#define CMD_CLOCK             0xffffff14UL
#define CMD_COLDSTART         0xffffff32UL
#define CMD_DIAL              0xffffff2dUL
#define CMD_DLSTART           0xffffff00UL
#define CMD_FGCOLOR           0xffffff0aUL
#define CMD_GAUGE             0xffffff13UL
#define CMD_GETMATRIX         0xffffff33UL
#define CMD_GETPTR            0xffffff23UL
#define CMD_GETPROPS		  0xffffff25UL
#define CMD_GRADCOLOR         0xffffff34UL
#define CMD_GRADIENT          0xffffff0bUL
#define CMD_INFLATE           0xffffff22UL
#define CMD_INTERRUPT         0xffffff02UL
#define CMD_KEYS              0xffffff0eUL
#define CMD_LOADIDENTITY      0xffffff26UL
#define CMD_LOADIMAGE         0xffffff24UL
#define CMD_LOGO              0xffffff31UL
#define CMD_MEMCPY            0xffffff1dUL
#define CMD_MEMCRC            0xffffff18UL
#define CMD_MEMSET            0xffffff1bUL
#define CMD_MEMWRITE          0xffffff1aUL
#define CMD_MEMZERO           0xffffff1cUL
#define CMD_NUMBER            0xffffff2eUL
#define CMD_PROGRESS          0xffffff0fUL
#define CMD_REGREAD           0xffffff19UL
#define CMD_ROTATE            0xffffff29UL
#define CMD_SCALE             0xffffff28UL
#define CMD_SCREENSAVER       0xffffff2fUL
#define CMD_SCROLLBAR         0xffffff11UL
#define CMD_SETFONT           0xffffff2bUL
#define CMD_SETMATRIX         0xffffff2aUL
#define CMD_SKETCH            0xffffff30UL
#define CMD_SLIDER            0xffffff10UL
#define CMD_SNAPSHOT          0xffffff1fUL
#define CMD_SPINNER           0xffffff16UL
#define CMD_STOP              0xffffff17UL
#define CMD_SWAP              0xffffff01UL
#define CMD_TEXT              0xffffff0cUL
#define CMD_TOGGLE            0xffffff12UL
#define CMD_TRACK             0xffffff2cUL
#define CMD_TRANSLATE         0xffffff27UL

// Display list commands to be embedded in Graphics Processor
#define DL_ALPHA_FUNC       	0x09000000UL // requires OR'd arguments
#define DL_BITMAP_HANDLE    	0x05000000UL // requires OR'd arguments
#define DL_BITMAP_LAYOUT    	0x07000000UL // requires OR'd arguments
#define DL_BITMAP_SIZE      	0x08000000UL // requires OR'd arguments
#define DL_BITMAP_SOURCE    	0x01000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_A   	0x15000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_B   	0x16000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_C   	0x17000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_D   	0x18000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_E   	0x19000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_F   	0x1A000000UL // requires OR'd arguments
#define DL_BLEND_FUNC       	0x0B000000UL // requires OR'd arguments
#define DL_BEGIN            	0x1F000000UL // requires OR'd arguments
#define DL_CALL             	0x1D000000UL // requires OR'd arguments
#define DL_CLEAR            	0x26000000UL // requires OR'd arguments
#define DL_CELL             	0x06000000UL // requires OR'd arguments
#define DL_CLEAR_RGB        	0x02000000UL // requires OR'd arguments
#define DL_CLEAR_STENCIL    	0x11000000UL // requires OR'd arguments
#define DL_CLEAR_TAG        	0x12000000UL // requires OR'd arguments
#define DL_COLOR_A          	0x0F000000UL // requires OR'd arguments
#define DL_COLOR_MASK       	0x20000000UL // requires OR'd arguments
#define DL_COLOR_RGB        	0x04000000UL // requires OR'd arguments
#define DL_DISPLAY          	0x00000000UL
#define DL_END              	0x21000000UL
#define DL_JUMP             	0x1E000000UL // requires OR'd arguments
#define DL_LINE_WIDTH       	0x0E000000UL // requires OR'd arguments
#define DL_MACRO            	0x25000000UL // requires OR'd arguments
#define DL_POINT_SIZE       	0x0D000000UL // requires OR'd arguments
#define DL_RESTORE_CONTEXT  	0x23000000UL
#define DL_RETURN           	0x24000000UL
#define DL_SAVE_CONTEXT     	0x22000000UL
#define DL_SCISSOR_SIZE     	0x1C000000UL // requires OR'd arguments
#define DL_SCISSOR_XY       	0x1B000000UL // requires OR'd arguments
#define DL_STENCIL_FUNC     	0x0A000000UL // requires OR'd arguments
#define DL_STENCIL_MASK     	0x13000000UL // requires OR'd arguments
#define DL_STENCIL_OP       	0x0C000000UL // requires OR'd arguments
#define DL_TAG             		0x03000000UL // requires OR'd arguments
#define DL_TAG_MASK         	0x14000000UL // requires OR'd arguments
#define DL_VERTEX2F         	0x40000000UL // requires OR'd arguments
#define DL_VERTEX2II        	0x02000000UL // requires OR'd arguments

// Command and register value options
#define CLR_COL               0x4
#define CLR_STN               0x2
#define CLR_TAG               0x1
#define DECR                  4UL
#define DECR_WRAP             7UL
#define DLSWAP_DONE           0UL
#define DLSWAP_FRAME          2UL
#define DLSWAP_LINE           1UL
#define DST_ALPHA             3UL
#define EDGE_STRIP_A          7UL
#define EDGE_STRIP_B          8UL
#define EDGE_STRIP_L          6UL
#define EDGE_STRIP_R          5UL
#define EQUAL                 5UL
#define GEQUAL                4UL
#define GREATER               3UL
#define INCR                  3UL
#define INCR_WRAP             6UL
#define INT_CMDEMPTY          32UL
#define INT_CMDFLAG           64UL
#define INT_CONVCOMPLETE      128UL
#define INT_PLAYBACK          16UL
#define INT_SOUND             8UL
#define INT_SWAP              1UL
#define INT_TAG               4UL
#define INT_TOUCH             2UL
#define INVERT                5UL
#define KEEP                  1UL
#define L1                    1UL
#define L4                    2UL
#define L8                    3UL
#define LEQUAL                2UL
#define LESS                  1UL
#define LINEAR_SAMPLES        0UL
#define LINES                 3UL
#define LINE_STRIP            4UL
#define NEAREST               0UL
#define NEVER                 0UL
#define NOTEQUAL              6UL
#define ONE                   1UL
#define ONE_MINUS_DST_ALPHA   5UL
#define ONE_MINUS_SRC_ALPHA   4UL
#define OPT_CENTER            1536UL    // 0x6000
#define OPT_CENTERX           512UL     // 0x0200
#define OPT_CENTERY           1024UL    // 0x0400
#define OPT_FLAT              256UL     // 0x0100
#define OPT_MONO              1UL
#define OPT_NOBACK            4096UL    // 0x1000
#define OPT_NODL              2UL
#define OPT_NOHANDS           49152UL   // 0xC168
#define OPT_NOHM              16384UL   // 0x4000
#define OPT_NOPOINTER         16384UL   // 0x4000
#define OPT_NOSECS            32768UL   // 0x8000
#define OPT_NOTICKS           8192UL    // 0x2000
#define OPT_RIGHTX            2048UL    // 0x0800
#define OPT_SIGNED            256UL     // 0x0100
#define PALETTED              8UL
#define PLAYCOLOR             0x00a0a080
#define FTPOINTS              2UL       // "POINTS" is a reserved word
#define RECTS                 9UL
#define REPEAT                1UL
#define REPLACE               2UL
#define RGB332                4UL
#define RGB565                7UL
#define SRC_ALPHA             2UL
#define TEXT8X8               9UL
#define TEXTVGA               10UL
#define TOUCHMODE_CONTINUOUS  3UL
#define TOUCHMODE_FRAME       2UL
#define TOUCHMODE_OFF         0UL
#define TOUCHMODE_ONESHOT     1UL
#define ULAW_SAMPLES          1UL
#define ZERO                  0UL

// Useful Macros
#define RGB(r, g, b)          ((((r) << 16) | (g) << 8) | (b))
#define SQ(v)                 ((v) * (v))
#define MIN(x,y)              ((x) > (y) ? (y) : (x))
#define MAX(x,y)              ((x) > (y) ? (x) : (y))
#define NOTE(n, sharp)        (((n) - 'C') + ((sharp) * 128))
#define F16(s)                (((s) * 65536))
#define INVALID_TOUCH_XY      0x8000
#define ABS(x)                ((x) > (0) ? (x) : (-x))

#define VERTEX2F(x,y) ((1UL<<30)|(((x)&32767UL)<<15)|(((y)&32767UL)<<0))
#define VERTEX2II(x,y,handle,cell) ((2UL<<30)|(((x)&511UL)<<21)|(((y)&511UL)<<12)|(((handle)&31UL)<<7)|(((cell)&127UL)<<0))
#define BITMAP_SOURCE(addr) ((1UL<<24)|(((addr)&1048575UL)<<0))
#define CLEAR_COLOR_RGB(red,green,blue) ((2UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define TAG(s) ((3UL<<24)|(((s)&255UL)<<0))
#define COLOR_RGB(red,green,blue) ((4UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define BITMAP_HANDLE(handle) ((5UL<<24)|(((handle)&31UL)<<0))
#define CELL(cell) ((6UL<<24)|(((cell)&127UL)<<0))
#define BITMAP_LAYOUT(format,linestride,height) ((7UL<<24)|(((format)&31UL)<<19)|(((linestride)&1023UL)<<9)|(((height)&511UL)<<0))
#define BITMAP_SIZE(filter,wrapx,wrapy,width,height) ((8UL<<24)|(((filter)&1UL)<<20)|(((wrapx)&1UL)<<19)|(((wrapy)&1UL)<<18)|(((width)&511UL)<<9)|(((height)&511UL)<<0))
#define ALPHA_FUNC(func,ref) ((9UL<<24)|(((func)&7UL)<<8)|(((ref)&255UL)<<0))
#define STENCIL_FUNC(func,ref,mask) ((10UL<<24)|(((func)&7UL)<<16)|(((ref)&255UL)<<8)|(((mask)&255UL)<<0))
#define BLEND_FUNC(src,dst) ((11UL<<24)|(((src)&7UL)<<3)|(((dst)&7UL)<<0))
#define STENCIL_OP(sfail,spass) ((12UL<<24)|(((sfail)&7UL)<<3)|(((spass)&7UL)<<0))
#define POINT_SIZE(size) ((13UL<<24)|(((size)&8191UL)<<0))
#define LINE_WIDTH(width) ((14UL<<24)|(((width)&4095UL)<<0))
#define CLEAR_COLOR_A(alpha) ((15UL<<24)|(((alpha)&255UL)<<0))
#define COLOR_A(alpha) ((16UL<<24)|(((alpha)&255UL)<<0))
#define CLEAR_STENCIL(s) ((17UL<<24)|(((s)&255UL)<<0))
#define CLEAR_TAG(s) ((18UL<<24)|(((s)&255UL)<<0))
#define STENCIL_MASK(mask) ((19UL<<24)|(((mask)&255UL)<<0))
#define TAG_MASK(mask) ((20UL<<24)|(((mask)&1UL)<<0))
#define BITMAP_TRANSFORM_A(a) ((21UL<<24)|(((a)&131071UL)<<0))
#define BITMAP_TRANSFORM_B(b) ((22UL<<24)|(((b)&131071UL)<<0))
#define BITMAP_TRANSFORM_C(c) ((23UL<<24)|(((c)&16777215UL)<<0))
#define BITMAP_TRANSFORM_D(d) ((24UL<<24)|(((d)&131071UL)<<0))
#define BITMAP_TRANSFORM_E(e) ((25UL<<24)|(((e)&131071UL)<<0))
#define BITMAP_TRANSFORM_F(f) ((26UL<<24)|(((f)&16777215UL)<<0))
#define SCISSOR_XY(x,y) ((27UL<<24)|(((x)&511UL)<<9)|(((y)&511UL)<<0))
#define SCISSOR_SIZE(width,height) ((28UL<<24)|(((width)&1023UL)<<10)|(((height)&1023UL)<<0))
#define CALL(dest) ((29UL<<24)|(((dest)&65535UL)<<0))
#define JUMP(dest) ((30UL<<24)|(((dest)&65535UL)<<0))
#define BEGIN(prim) ((31UL<<24)|(((prim)&15UL)<<0))
#define COLOR_MASK(r,g,b,a) ((32UL<<24)|(((r)&1UL)<<3)|(((g)&1UL)<<2)|(((b)&1UL)<<1)|(((a)&1UL)<<0))
#define CLEAR(c,s,t) ((38UL<<24)|(((c)&1UL)<<2)|(((s)&1UL)<<1)|(((t)&1UL)<<0))
#define END() ((33UL<<24))
#define SAVE_CONTEXT() ((34UL<<24))
#define RESTORE_CONTEXT() ((35UL<<24))
#define RETURN() ((36UL<<24))
#define MACRO(m) ((37UL<<24)|(((m)&1UL)<<0))
#define DISPLAY() ((0UL<<24))

#define POINTS               2UL
#define BITMAPS              1UL

#endif  //__ft_gpu_h
