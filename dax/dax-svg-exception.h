/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DAX_SVG_EXCEPTION_H__
#define __DAX_SVG_EXCEPTION_H__

#include <glib.h>

G_BEGIN_DECLS

#define DAX_SVG_EXCEPTION_ERROR    dax_exception_error_quark()

typedef enum /*< skip >*/
{
    DAX_SVG_EXCEPTION_ERROR_WRONG_TYPE,
    DAX_SVG_EXCEPTION_ERROR_INVALID_VALUE,
    DAX_SVG_EXCEPTION_ERROR_MATRIX_NOT_INVERTABLE,
} DaxSvgExceptionError;

GQuark      dax_svg_exception_error_quark    (void);

G_END_DECLS

#endif /* __DAX_SVG_EXCEPTION_H__ */
