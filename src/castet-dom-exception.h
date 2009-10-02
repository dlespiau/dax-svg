/*
 * Castet - Load and draw SVG
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __CASTET_DOM_EXCEPTION_H__
#define __CASTET_DOM_EXCEPTION_H__

#include <glib.h>

G_BEGIN_DECLS

#define CASTET_DOM_EXCEPTION_ERROR    castet_dom_exception_error_quark()

enum _CastetDomExceptionError {
    CASTET_DOM_EXCEPTION_ERROR_INDEX_SIZE = 1,
    CASTET_DOM_EXCEPTION_ERROR_DOMSTRING_SIZE,
    CASTET_DOM_EXCEPTION_ERROR_HIERARCHY_REQUEST,
    CASTET_DOM_EXCEPTION_ERROR_WRONG_DOCUMENT,
    CASTET_DOM_EXCEPTION_ERROR_INVALID_CHARACTER,
    CASTET_DOM_EXCEPTION_ERROR_NO_DATA_ALLOWED,
    CASTET_DOM_EXCEPTION_ERROR_NO_MODIFICATION_ALLOWED,
    CASTET_DOM_EXCEPTION_ERROR_NOT_FOUND,
    CASTET_DOM_EXCEPTION_ERROR_NOT_SUPPORTED,
    CASTET_DOM_EXCEPTION_ERROR_INUSE_ATTRIBUTE,
    CASTET_DOM_EXCEPTION_ERROR_INVALID_STATE,
    CASTET_DOM_EXCEPTION_ERROR_SYNTAX,
    CASTET_DOM_EXCEPTION_ERROR_INVALID_MODIFICATION,
    CASTET_DOM_EXCEPTION_ERROR_NAMESPACE,
    CASTET_DOM_EXCEPTION_ERROR_INVALID_ACCESS,
    CASTET_DOM_EXCEPTION_ERROR_VALIDATION,
    CASTET_DOM_EXCEPTION_ERROR_TYPE_MISMATCH
};
typedef enum _CastetDomExceptionError CastetDomExceptionError;

GQuark castet_dom_exception_error_quark(void);

G_END_DECLS

#endif /* __CASTET_DOM_EXCEPTION_H__ */
