/* format.h - v0.90 - MIT - Björn Nilsson, 2019
 *
 * LICENSE
 *   See end of file for license information.
 */

#ifndef FORMAT_H_INCLUDED
#define FORMAT_H_INCLUDED

#define BNI_FAM_TYPE        0xFFFF000000000000
#define BNI_FAM_FORMAT      0x0000FFFF00000000
#define BNI_FAM_PRECISION   0x00000000FFFF0000
#define BNI_FAM_WIDTH       0x000000000000FFFF

// format flags
#define BNI_FA_SIENTIFIC    0x0000000100000000
#define BNI_FA_HEX          0x0000000200000000
#define BNI_FA_EXP          0x0000000400000000
#define BNI_FA_GROUP        0x0000000800000000
#define BNI_FA_PAD_ZERO     0x0000001000000000
#define BNI_FA_SPACE        0x0000002000000000
#define BNI_FA_JUSTIFY_LEFT 0x0000004000000000
#define BNI_FA_PLUS         0x0000008000000000

#define BNI_FAT_UNSIGNED    0xF000000000000000
#define BNI_FAT_SIGNED      0x0F00000000000000
#define BNI_FAT_IEEE        0x00C0000000000000

// datatype
#define BNI_FAT_U64         0x8000000000000000
#define BNI_FAT_U32         0x4000000000000000
#define BNI_FAT_U16         0x2000000000000000
#define BNI_FAT_U8          0x1000000000000000
#define BNI_FAT_S64         0x0800000000000000
#define BNI_FAT_S32         0x0400000000000000
#define BNI_FAT_S16         0x0200000000000000
#define BNI_FAT_S8          0x0100000000000000
#define BNI_FAT_F64         0x0080000000000000
#define BNI_FAT_F32         0x0040000000000000
#define BNI_FAT_PTR         0x0020000000000000
#define BNI_FAT_STR         0x0010000000000000
#define BNI_FAT_CSTR        0x0008000000000000

#define BNI_FAC_C__L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_SPACE)
#define BNI_FAC_D__L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PLUS)
#define BNI_FAC__Z_L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PAD_ZERO)
#define BNI_FAC_CZ_L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_SPACE | BNI_FA_PAD_ZERO)
#define BNI_FAC_DZ_L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PLUS  | BNI_FA_PAD_ZERO)
#define BNI_FAC__ZXL  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PAD_ZERO | BNI_FA_HEX)
#define BNI_FAC__ZOL  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PAD_ZERO | BNI_FA_OCT)
#define BNI_FAC___XL  (BNI_FA_JUSTIFY_LEFT | BNI_FA_HEX)
#define BNI_FAC___OL  (BNI_FA_JUSTIFY_LEFT | BNI_FA_OCT)
#define BNI_FAC____L  (BNI_FA_JUSTIFY_LEFT)

#define BNI_FAC_C___ (BNI_FA_SPACE)
#define BNI_FAC_D___ (BNI_FA_PLUS)
#define BNI_FAC__Z__ (BNI_FA_PAD_ZERO)
#define BNI_FAC_CZ__ (BNI_FA_SPACE | BNI_FA_PAD_ZERO)
#define BNI_FAC_DZ__ (BNI_FA_PLUS  | BNI_FA_PAD_ZERO)
#define BNI_FAC__ZX_ (BNI_FA_PAD_ZERO | BNI_FA_HEX)
#define BNI_FAC__ZO_ (BNI_FA_PAD_ZERO | BNI_FA_OCT)
#define BNI_FAC___X_ (BNI_FA_HEX)
#define BNI_FAC___O_ (BNI_FA_OCT)
#define BNI_FAC_____ 0

/* Unsigned formaters */
#define bni_fa_clu(val,width)  bni_format_arg_u(BNI_FAC_C__L,(val),(width),0)
#define bni_fa_dlu(val,width)  bni_format_arg_u(BNI_FAC_D__L,(val),(width),0)
#define bni_fa_zlu(val,width)  bni_format_arg_u(BNI_FAC__Z_L,(val),(width),0)
#define bni_fa_czlu(val,width) bni_format_arg_u(BNI_FAC_CZ_L,(val),(width),0)
#define bni_fa_dzlu(val,width) bni_format_arg_u(BNI_FAC_DZ_L,(val),(width),0)
#define bni_fa_zxlu(val,width) bni_format_arg_u(BNI_FAC__ZXL,(val),(width),0)
#define bni_fa_zolu(val,width) bni_format_arg_u(BNI_FAC__ZOL,(val),(width),0)
#define bni_fa_xlu(val,width)  bni_format_arg_u(BNI_FAC___XL,(val),(width),0)
#define bni_fa_olu(val,width)  bni_format_arg_u(BNI_FAC___OL,(val),(width),0)
#define bni_fa_lu(val,width)   bni_format_arg_u(BNI_FAC____L,(val),(width),0)

#define bni_fa_zu(val,width)   bni_format_arg_u(BNI_FAC__Z__,(val),(width),0)
#define bni_fa_czu(val,width)  bni_format_arg_u(BNI_FAC_CZ__,(val),(width),0)
#define bni_fa_dzu(val,width)  bni_format_arg_u(BNI_FAC_DZ__,(val),(width),0)
#define bni_fa_zxu(val,width)  bni_format_arg_u(BNI_FAC__ZX_,(val),(width),0)
#define bni_fa_zou(val,width)  bni_format_arg_u(BNI_FAC__ZO_,(val),(width),0)
#define bni_fa_xru(val,width)  bni_format_arg_u(BNI_FAC___X_,(val),(width),0)
#define bni_fa_oru(val,width)  bni_format_arg_u(BNI_FAC___O_,(val),(width),0)
#define bni_fa_cru(val,width)  bni_format_arg_u(BNI_FAC_C___,(val),(width),0)
#define bni_fa_dru(val,width)  bni_format_arg_u(BNI_FAC_D___,(val),(width),0)

#define bni_fa_u(val)          bni_format_arg_u(BNI_FAC_____,(val),0,0)
#define bni_fa_cu(val)         bni_format_arg_u(BNI_FAC_C___,(val),0,0)
#define bni_fa_du(val)         bni_format_arg_u(BNI_FAC_D___,(val),0,0)
#define bni_fa_xu(val)         bni_format_arg_u(BNI_FAC___X_,(val),0,0)
#define bni_fa_ou(val)         bni_format_arg_u(BNI_FAC___O_,(val),0,0)

/* Signed formaters */
#define bni_fa_cli(val,width)  bni_format_arg_i(BNI_FAC_C__L,(val),(width),0)
#define bni_fa_dli(val,width)  bni_format_arg_i(BNI_FAC_D__L,(val),(width),0)
#define bni_fa_zli(val,width)  bni_format_arg_i(BNI_FAC__Z_L,(val),(width),0)
#define bni_fa_czli(val,width) bni_format_arg_i(BNI_FAC_CZ_L,(val),(width),0)
#define bni_fa_dzli(val,width) bni_format_arg_i(BNI_FAC_DZ_L,(val),(width),0)
#define bni_fa_zxli(val,width) bni_format_arg_i(BNI_FAC__ZXL,(val),(width),0)
#define bni_fa_zoli(val,width) bni_format_arg_i(BNI_FAC__ZOL,(val),(width),0)
#define bni_fa_xli(val,width)  bni_format_arg_i(BNI_FAC___XL,(val),(width),0)
#define bni_fa_oli(val,width)  bni_format_arg_i(BNI_FAC___OL,(val),(width),0)
#define bni_fa_li(val,width)   bni_format_arg_i(BNI_FAC____L,(val),(width),0)

#define bni_fa_zi(val,width)   bni_format_arg_i(BNI_FAC__Z__,(val),(width),0)
#define bni_fa_czi(val,width)  bni_format_arg_i(BNI_FAC_CZ__,(val),(width),0)
#define bni_fa_dzi(val,width)  bni_format_arg_i(BNI_FAC_DZ__,(val),(width),0)
#define bni_fa_zxi(val,width)  bni_format_arg_i(BNI_FAC__ZX_,(val),(width),0)
#define bni_fa_zoi(val,width)  bni_format_arg_i(BNI_FAC__ZO_,(val),(width),0)
#define bni_fa_xri(val,width)  bni_format_arg_i(BNI_FAC___X_,(val),(width),0)
#define bni_fa_ori(val,width)  bni_format_arg_i(BNI_FAC___O_,(val),(width),0)
#define bni_fa_cri(val,width)  bni_format_arg_i(BNI_FAC_C___,(val),(width),0)
#define bni_fa_dri(val,width)  bni_format_arg_i(BNI_FAC_D___,(val),(width),0)

#define bni_fa_i(val)          bni_format_arg_i(BNI_FAC_____,(val),0,0)
#define bni_fa_ci(val)         bni_format_arg_i(BNI_FAC_C___,(val),0,0)
#define bni_fa_di(val)         bni_format_arg_i(BNI_FAC_D___,(val),0,0)
#define bni_fa_xi(val)         bni_format_arg_i(BNI_FAC___X_,(val),0,0)
#define bni_fa_oi(val)         bni_format_arg_i(BNI_FAC___O_,(val),0,0)

/* Pointer formaters */
#define bni_fa_p(val)          bni_format_arg_p(BNI_FAC__ZX_,(val),sizeof(size_t),0)
#define bni_fa_rp(val,width)   bni_format_arg_p(BNI_FAC__ZX_,(val),(width),0)
#define bni_fa_lp(val,width)   bni_format_arg_p(BNI_FAC__ZXL,(val),(width),0)

/* cstring formaters */
#define bni_fa_s(val)                   bni_format_arg_s(BNI_FAC_____,(val),0, 0)
#define bni_fa_xs(val)                  bni_format_arg_s(BNI_FAC___X_,(val),0, 0)
#define bni_fa_xs_(val,width,precision) bni_format_arg_s(BNI_FAC___X_,(val),(width),(precision))
#define bni_fa_ls(val,width)            bni_format_arg_s(BNI_FAC____L,(val),(width), 0)
#define bni_fa_rs(val,width)            bni_format_arg_s(BNI_FAC_____,(val),(width), 0)
#define bni_fa_s_(val,width,precision)  bni_format_arg_s(BNI_FAC_____,(val),(width),(precision))

/* string (text) formaters */
#define bni_fa_t(val)                   bni_format_arg_s(BNI_FAC_____,(val)->str,0, (val)->len)
#define bni_fa_xt(val)                  bni_format_arg_s(BNI_FAC___X_,(val)->str,0, (val)->len)
#define bni_fa_xt_(val,width,precision) bni_format_arg_s(BNI_FAC___X_,(val)->str,(width),(precision))
#define bni_fa_lt(val,width)            bni_format_arg_s(BNI_FAC____L,(val)->str,(width), (val)->len)
#define bni_fa_rt(val,width)            bni_format_arg_s(BNI_FAC_____,(val)->str,(width), (val)->len)
#define bni_fa_t_(val,width,precision)  bni_format_arg_s(BNI_FAC_____,(val)->str,(width),(precision))

/* Float formaters */
#define bni_fa_f(val)          bni_format_arg_f(BNI_FAC_____,(val),0,6)
#define bni_fa_cf(val)         bni_format_arg_f(BNI_FAC_C___,(val),0,6)
#define bni_fa_df(val)         bni_format_arg_f(BNI_FAC_D___,(val),0,6)

#define bni_fa_crf(val,width)  bni_format_arg_f(BNI_FAC_C___,(val),(width),6)
#define bni_fa_drf(val,width)  bni_format_arg_f(BNI_FAC_D___,(val),(width),6)
#define bni_fa_zf(val,width)   bni_format_arg_f(BNI_FAC__Z__,(val),(width),6)
#define bni_fa_czf(val,width)  bni_format_arg_f(BNI_FAC_CZ__,(val),(width),6)
#define bni_fa_dzf(val,width)  bni_format_arg_f(BNI_FAC_DZ__,(val),(width),6)

#define bni_fa_lf(val,width)   bni_format_arg_f(BNI_FAC____L,(val),(width),6)
#define bni_fa_clf(val,width)  bni_format_arg_f(BNI_FAC_C__L,(val),(width),6)
#define bni_fa_dlf(val,width)  bni_format_arg_f(BNI_FAC_D__L,(val),(width),6)
#define bni_fa_zlf(val,width)  bni_format_arg_f(BNI_FAC__Z_L,(val),(width),6)
#define bni_fa_czlf(val,width) bni_format_arg_f(BNI_FAC_CZ_L,(val),(width),6)
#define bni_fa_dzlf(val,width) bni_format_arg_f(BNI_FAC_DZ_L,(val),(width),6)

#define bni_fa_xf(val)         bni_format_arg_u(BNI_FAC__ZX_,(*(u32*)&(val)),8,0)

#define bni_fa_e(val)          bni_format_arg_f(BNI_FA_EXP | BNI_FAC_____,(val),0,6)
#define bni_fa_ce(val)         bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C___,(val),0,6)
#define bni_fa_de(val)         bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D___,(val),0,6)
#define bni_fa_re(val,width)   bni_format_arg_f(BNI_FA_EXP | BNI_FAC_____,(val),(width),6)
#define bni_fa_cre(val,width)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C___,(val),(width),6)
#define bni_fa_dre(val,width)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D___,(val),(width),6)
#define bni_fa_le(val,width)   bni_format_arg_f(BNI_FA_EXP | BNI_FAC____L,(val),(width),6)
#define bni_fa_cle(val,width)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C__L,(val),(width),6)
#define bni_fa_dle(val,width)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D__L,(val),(width),6)

/* _-suffix exposes precision field */
#define bni_fa_rf_(val,width,precision)   bni_format_arg_f(BNI_FAC_____,(val),(width),(precision))
#define bni_fa_crf_(val,width,precision)  bni_format_arg_f(BNI_FAC_C___,(val),(width),(precision))
#define bni_fa_drf_(val,width,precision)  bni_format_arg_f(BNI_FAC_D___,(val),(width),(precision))
#define bni_fa_zf_(val,width,precision)   bni_format_arg_f(BNI_FAC__Z__,(val),(width),(precision))
#define bni_fa_czf_(val,width,precision)  bni_format_arg_f(BNI_FAC_CZ__,(val),(width),(precision))
#define bni_fa_dzf_(val,width,precision)  bni_format_arg_f(BNI_FAC_DZ__,(val),(width),(precision))

#define bni_fa_lf_(val,width,precision)   bni_format_arg_f(BNI_FAC____L,(val),(width),(precision))
#define bni_fa_clf_(val,width,precision)  bni_format_arg_f(BNI_FAC_C__L,(val),(width),(precision))
#define bni_fa_dlf_(val,width,precision)  bni_format_arg_f(BNI_FAC_D__L,(val),(width),(precision))
#define bni_fa_zlf_(val,width,precision)  bni_format_arg_f(BNI_FAC__Z_L,(val),(width),(precision))
#define bni_fa_czlf_(val,width,precision) bni_format_arg_f(BNI_FAC_CZ_L,(val),(width),(precision))
#define bni_fa_dzlf_(val,width,precision) bni_format_arg_f(BNI_FAC_DZ_L,(val),(width),(precision))

#define bni_fa_re_(val,width,precision)   bni_format_arg_f(BNI_FA_EXP | BNI_FAC_____,(val),(width),(precision))
#define bni_fa_cre_(val,width,precision)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C___,(val),(width),(precision))
#define bni_fa_dre_(val,width,precision)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D___,(val),(width),(precision))
#define bni_fa_le_(val,width,precision)   bni_format_arg_f(BNI_FA_EXP | BNI_FAC____L,(val),(width),(precision))
#define bni_fa_cle_(val,width,precision)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C__L,(val),(width),(precision))
#define bni_fa_dle_(val,width,precision)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D__L,(val),(width),(precision))



typedef struct BniStringFormatArg {
	u64 format_info;
	union {
		u64 u;
		i64 s;
		f64 d;
		f32 f;
		void *p;
		char *c_str;
	} v;

} BniStringFormatArg;

// 16 bytes + array of 16 bytes
typedef struct BniStringFormater {
//	BniStringFormatArg *args;
	i8 *format_string;
	u32 arg_max;
	u32 arg_count;
	BniStringFormatArg args[0];
} BniStringFormater;

internal void bni_fprintf(OSFile f, i8 *fmt, ...);
internal void bni_printf(i8 *fmt, ...);
internal inline BniStringFormater * formater_alloc(Arena *arena, u32 max, i8 *format_string);
internal u32 bni_format_integer(u8 *out, umm size, BniStringFormatArg *arg);

#endif //FORMAT_H_INCLUDED

/* Copyright 2019 Björn Nilsson <public@bnicon.se> 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 // vim: ts=4 sw=4 cindent cino=0(,\:0,l1
