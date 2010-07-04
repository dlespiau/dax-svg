/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
 *
 * This file was taken from librsvg, rsvg-private.h, LGPLv2+.
 * The rsvg namespace was renamed to dax.
 */

/*
   rsvg-private.h: Internals of RSVG

   Copyright (C) 2000 Eazel, Inc.
   Copyright (C) 2002 Dom Lachowicz <cinamod@hotmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.

   Author: Raph Levien <raph@artofcode.com>
*/

#include <glib.h>

#ifndef __DAX_AFFINE_H__
#define __DAX_AFFINE_H__

G_BEGIN_DECLS

void    _dax_affine_invert          (double       dst_affine[6],
                                     const double src_affine[6]);
void    _dax_affine_flip            (double       dst_affine[6],
                                     const double src_affine[6],
                                     int          horz,
                                     int          vert);
void    _dax_affine_multiply        (double       dst[6],
                                     const double src1[6],
                                     const double src2[6]);
void    _dax_affine_identity        (double dst[6]);
void    _dax_affine_scale           (double dst[6],
                                     double sx,
                                     double sy);
void    _dax_affine_rotate          (double dst[6],
                                     double theta);
void    _dax_affine_shear           (double dst[6],
                                     double theta);
void    _dax_affine_translate       (double dst[6],
                                     double tx,
                                     double ty);
double  _dax_affine_expansion       (const double src[6]);
int     _dax_affine_rectilinear     (const double src[6]);
int     _dax_affine_equal           (double matrix1[6],
                                     double matrix2[6]);

G_END_DECLS

#endif
