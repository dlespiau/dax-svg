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

#ifndef __DAX_DOM_EXCEPTION_H__
#define __DAX_DOM_EXCEPTION_H__

#include <glib.h>

G_BEGIN_DECLS

#define DAX_DOM_EXCEPTION_ERROR    dax_dom_exception_error_quark()

typedef enum /*< skip >*/
{
    DAX_DOM_EXCEPTION_ERROR_INDEX_SIZE = 1,
    DAX_DOM_EXCEPTION_ERROR_DOMSTRING_SIZE,
    DAX_DOM_EXCEPTION_ERROR_HIERARCHY_REQUEST,
    DAX_DOM_EXCEPTION_ERROR_WRONG_DOCUMENT,
    DAX_DOM_EXCEPTION_ERROR_INVALID_CHARACTER,
    DAX_DOM_EXCEPTION_ERROR_NO_DATA_ALLOWED,
    DAX_DOM_EXCEPTION_ERROR_NO_MODIFICATION_ALLOWED,
    DAX_DOM_EXCEPTION_ERROR_NOT_FOUND,
    DAX_DOM_EXCEPTION_ERROR_NOT_SUPPORTED,
    DAX_DOM_EXCEPTION_ERROR_INUSE_ATTRIBUTE,
    DAX_DOM_EXCEPTION_ERROR_INVALID_STATE,
    DAX_DOM_EXCEPTION_ERROR_SYNTAX,
    DAX_DOM_EXCEPTION_ERROR_INVALID_MODIFICATION,
    DAX_DOM_EXCEPTION_ERROR_NAMESPACE,
    DAX_DOM_EXCEPTION_ERROR_INVALID_ACCESS,
    DAX_DOM_EXCEPTION_ERROR_VALIDATION,
    DAX_DOM_EXCEPTION_ERROR_TYPE_MISMATCH
} DaxDomExceptionError;

GQuark dax_dom_exception_error_quark(void);

G_END_DECLS

#endif /* __DAX_DOM_EXCEPTION_H__ */
