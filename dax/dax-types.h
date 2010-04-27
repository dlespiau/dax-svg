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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(__DAX_H_INSIDE__) && !defined(DAX_COMPILATION)
#error "Only <dax/dax.h> can be included directly."
#endif

#ifndef __DAX_TYPES_H__
#define __DAX_TYPES_H__

#include <glib-object.h>

G_BEGIN_DECLS

/*
 * Animation types
 */

/*
 * DaxDuration
 */

#define DAX_TYPE_DURATION        (dax_duration_get_type ())
#define DAX_VALUE_HOLDS_DURATION (G_VALUE_HOLDS ((x), DAX_TYPE_DURATION))

typedef enum /*< skip >*/
{
    DAX_DURATION_S,
    DAX_DURATION_MS
} DaxDurationType;

typedef struct _DaxDuration DaxDuration;

struct _DaxDuration
{
    DaxDurationType unit_type;

    gfloat value;

    gfloat ms;
    guint ms_set;
};

GType               dax_duration_get_type        (void) G_GNUC_CONST;
DaxDurationType  dax_duration_get_unit_type   (const DaxDuration *dur);
gfloat              dax_duration_get_unit_value  (const DaxDuration *dur);

DaxDuration *    dax_duration_copy            (const DaxDuration *dur);
void                dax_duration_free            (DaxDuration *dur);

void                dax_duration_from_s          (DaxDuration *dur,
                                                     gfloat          seconds);
void                dax_duration_from_ms         (DaxDuration *dur,
                                                     gfloat          ms);

gfloat              dax_duration_to_ms           (DaxDuration *dur);

gboolean            dax_duration_from_string     (DaxDuration *dur,
                                                     const gchar    *string);
gchar *             dax_duration_to_string       (const DaxDuration *dur);

/*
 * DaxAnimationAttributeType
 */

#define DAX_ANIMATION_ATTRIBUTE_TYPE_CASE_SENSITIVE  1
#define DAX_ANIMATION_ATTRIBUTE_TYPE_DEFAULT         \
    DAX_ANIMATION_ATTRIBUTE_TYPE_AUTO

typedef enum _DaxAnimationAttributeType
{
    DAX_ANIMATION_ATTRIBUTE_TYPE_AUTO,
    DAX_ANIMATION_ATTRIBUTE_TYPE_CSS,    /*< nick=CSS >*/
    DAX_ANIMATION_ATTRIBUTE_TYPE_XML     /*< nick=XML >*/
} DaxAnimationAttributeType;

/*
 * DaxRepeatCount
 */

#define DAX_TYPE_REPEAT_COUNT        (dax_repeat_count_get_type ())
#define DAX_VALUE_HOLDS_REPEAT_COUNT \
    (G_VALUE_HOLDS ((x), DAX_TYPE_REPEAT_COUNT))

typedef struct _DaxRepeatCount DaxRepeatCount;

struct _DaxRepeatCount
{
    gfloat value;
};

GType               dax_repeat_count_get_type        (void) G_GNUC_CONST;

DaxRepeatCount * dax_repeat_count_copy            (const DaxRepeatCount *count);
void                dax_repeat_count_free            (DaxRepeatCount *count);

void                dax_repeat_count_from_number     (DaxRepeatCount *count,
                                                         gfloat             ms);

gboolean            dax_repeat_count_is_indefinite   (const DaxRepeatCount *count);
gfloat              dax_repeat_count_get_value       (const DaxRepeatCount *count);

gboolean            dax_repeat_count_from_string     (DaxRepeatCount *count,
                                                         const gchar       *string);
gchar *             dax_repeat_count_to_string       (const DaxRepeatCount *count);

G_END_DECLS

#endif /* __DAX_TYPES_H__ */
