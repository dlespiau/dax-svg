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

#ifndef __DAX_KNOT_SEQUENCE_H__
#define __DAX_KNOT_SEQUENCE_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define DAX_TYPE_KNOT_SEQUENCE dax_knot_sequence_get_type()

#define DAX_KNOT_SEQUENCE(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_KNOT_SEQUENCE, \
                                 DaxKnotSequence))

#define DAX_KNOT_SEQUENCE_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              DAX_TYPE_KNOT_SEQUENCE,    \
                              DaxKnotSequenceClass))

#define DAX_IS_KNOT_SEQUENCE(obj)    \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_KNOT_SEQUENCE))

#define DAX_IS_KNOT_SEQUENCE_CLASS(klass)    \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),          \
                              DAX_TYPE_KNOT_SEQUENCE))

#define DAX_KNOT_SEQUENCE_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_KNOT_SEQUENCE,  \
                                DaxKnotSequenceClass))

typedef struct _DaxKnotSequence DaxKnotSequence;
typedef struct _DaxKnotSequenceClass DaxKnotSequenceClass;
typedef struct _DaxKnotSequencePrivate DaxKnotSequencePrivate;

struct _DaxKnotSequence
{
    GObject parent;

    DaxKnotSequencePrivate *priv;
};

struct _DaxKnotSequenceClass
{
    GObjectClass parent_class;
};

GType               dax_knot_sequence_get_type               (void) G_GNUC_CONST;

DaxKnotSequence *dax_knot_sequence_new                    (void);
DaxKnotSequence *dax_knot_sequence_new_from_static_array  (gfloat *data,
                                                                 guint   nb_knots);
DaxKnotSequence *dax_knot_sequence_new_from_string        (const gchar *string);
gchar *             dax_knot_sequence_to_string              (const DaxKnotSequence *seq);

guint               dax_knot_sequence_get_size               (const DaxKnotSequence *seq);
const gfloat *      dax_knot_sequence_get_array              (const DaxKnotSequence *seq);

G_END_DECLS

#endif /* __DAX_KNOT_SEQUENCE_H__ */
