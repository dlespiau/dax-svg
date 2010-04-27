/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Forward declaration to break declaration cycles of DaxXml* elements
 */

#ifndef __DAX_XML_FORWARD_H__
#define __DAX_XML_FORWARD_H__

#include <glib.h>

G_BEGIN_DECLS

/* dax-xml-event.h */
typedef union _DaxXmlEvent DaxXmlEvent;

/* dax-xml-event-listener.h */
typedef struct _DaxXmlEventListener DaxXmlEventListener;

/* dax-xml-event-target.h */
typedef struct _DaxXmlEventTarget DaxXmlEventTarget; /* dummy typedef */

G_END_DECLS

#endif /* __DAX_XML_FORWARD_H__ */
