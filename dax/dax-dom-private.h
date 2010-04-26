/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
 *
 * Authored by: Damien Lespiau <damien.lespiau@intel.com>
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

#ifndef __DAX_DOM_PRIVATE_H__
#define __DAX_DOM_PRIVATE_H__

#include <glib.h>

#include "dax-xml-private.h"
#include "dax-xml-event.h"
#include "dax-dom-document.h"
#include "dax-dom-element.h"

G_BEGIN_DECLS

const gchar *xml_ns;
const gchar *xmlns_ns;
const gchar *xmlevents_ns;

/* dax-dom-document.c */

void            _dax_dom_document_add_namespace                 (DaxDomDocument *document,
                                                                 const gchar    *uri,
                                                                 const gchar    *prefix);
void            _dax_dom_document_add_namespace_static          (DaxDomDocument *document,
                                                                 const gchar    *uri,
                                                                 const gchar    *prefix);
const gchar *   _dax_dom_document_get_prefix_for_interned_uri   (DaxDomDocument *document,
                                                                 const gchar    *uri);

gboolean        _dax_dom_document_set_element_id    (DaxDomDocument *document,
                                                     DaxDomElement  *element,
                                                     const gchar    *id);
void            _dax_dom_document_unset_id          (DaxDomDocument *document,
                                                     const gchar    *id);

/* dax-dom-element.c */

void            _dax_dom_element_handle_event   (DaxDomElement *element,
                                                 DaxXmlEvent   *event);
void            _dax_dom_element_signal_parsed  (DaxDomElement *element);

G_END_DECLS

#endif /* __DAX_DOM_PRIVATE_H__ */
