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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __DAX_DOM_PARSER_H__
#define __DAX_DOM_PARSER_H__

#include "dax-dom-document.h"

#include <glib.h>

G_BEGIN_DECLS

DaxDomDocument *    dax_dom_document_new_from_file      (const gchar  *filename,
                                                         GError      **error);
DaxDomDocument *    dax_dom_document_new_from_memory    (const gchar  *buffer,
                                                         gint          size,
                                                         const gchar  *base_url,
                                                         GError      **error);

G_END_DECLS

#endif /* __DAX_DOM_PARSER_H__ */
