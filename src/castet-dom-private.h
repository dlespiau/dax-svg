/*
 * Castet - Load and draw SVG
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

#ifndef __CASTET_DOM_PRIVATE_H__
#define __CASTET_DOM_PRIVATE_H__

#include <glib.h>

#include "castet-xml-private.h"
#include "castet-xml-event.h"
#include "castet-dom-document.h"
#include "castet-dom-element.h"

G_BEGIN_DECLS

const gchar *xml_ns;
const gchar *xmlns_ns;
const gchar *xmlevents_ns;

/* castet-dom-document.c */

void            _castet_dom_document_add_namespace                  (CastetDomDocument *document,
                                                                     const gchar       *uri,
                                                                     const gchar       *prefix);
void            _castet_dom_document_add_namespace_static           (CastetDomDocument *document,
                                                                     const gchar       *uri,
                                                                     const gchar       *prefix);
const gchar *   _castet_dom_document_get_prefix_for_interned_uri    (CastetDomDocument *document,
                                                                     const gchar       *uri);


/* castet-dom-element.c */

void            _castet_dom_element_handle_event                    (CastetDomElement *element,
                                                                     CastetXmlEvent   *event);

G_END_DECLS

#endif /* __CASTET_DOM_PRIVATE_H__ */
