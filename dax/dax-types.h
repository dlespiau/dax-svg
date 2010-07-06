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
#include <clutter/clutter.h>

G_BEGIN_DECLS

/*
 * DaxMatrix
 */

#define DAX_TYPE_MATRIX        (dax_matrix_get_type ())
#define DAX_VALUE_HOLDS_MATRIX (G_VALUE_HOLDS ((x), DAX_TYPE_MATRIX))

typedef struct _DaxMatrix DaxMatrix;
typedef struct _DaxElementaryMatrix DaxElementaryMatrix;

typedef enum /*< skip >*/
{
    DAX_MATRIX_TYPE_GENERIC,       /* free form matrix */
    DAX_MATRIX_TYPE_ROTATE,
    DAX_MATRIX_TYPE_ROTATE_AROUND,
    DAX_MATRIX_TYPE_TRANSLATE,
    DAX_MATRIX_TYPE_SCALE,
    DAX_MATRIX_TYPE_SKEW_X,
    DAX_MATRIX_TYPE_SKEW_Y
} DaxMatrixType;

struct _DaxElementaryMatrix
{
    DaxMatrixType type;
    float params[3];                            /* up to 3 paramaters */
};

struct _DaxMatrix
{
    volatile gint refcount;
    GList *elementary_matrices;
    double affine[6];
};

GType           dax_matrix_get_type         (void) G_GNUC_CONST;

DaxMatrix *     dax_matrix_copy             (DaxMatrix *matrix);
DaxMatrix *     dax_matrix_deep_copy        (const DaxMatrix *matrix);
void            dax_matrix_free             (DaxMatrix *matrix);

void            dax_matrix_from_array       (DaxMatrix *matrix,
                                             double     src[6]);

double *        dax_matrix_get_affine       (DaxMatrix *matrix);

gboolean        dax_matrix_from_string      (DaxMatrix   *matrix,
                                             const gchar *string);
gchar *         dax_matrix_to_string        (const DaxMatrix *matrix);
void            dax_matrix_transform_point  (const DaxMatrix    *matrix,
                                             const ClutterPoint *point,
                                             ClutterPoint       *out);

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
DaxDurationType     dax_duration_get_unit_type   (const DaxDuration *dur);
gfloat              dax_duration_get_unit_value  (const DaxDuration *dur);

DaxDuration *       dax_duration_copy            (const DaxDuration *dur);
void                dax_duration_free            (DaxDuration *dur);

void                dax_duration_from_s          (DaxDuration *dur,
                                                  gfloat       seconds);
void                dax_duration_from_ms         (DaxDuration *dur,
                                                  gfloat       ms);

gfloat              dax_duration_to_ms           (DaxDuration *dur);

gboolean            dax_duration_from_string     (DaxDuration *dur,
                                                  const gchar *string);
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

GType               dax_repeat_count_get_type       (void) G_GNUC_CONST;

DaxRepeatCount *    dax_repeat_count_copy           (const DaxRepeatCount *count);
void                dax_repeat_count_free           (DaxRepeatCount *count);

void                dax_repeat_count_from_number    (DaxRepeatCount *count,
                                                     gfloat          ms);

gboolean            dax_repeat_count_is_indefinite  (const DaxRepeatCount *count);
gfloat              dax_repeat_count_get_value      (const DaxRepeatCount *count);

gboolean            dax_repeat_count_from_string    (DaxRepeatCount *count,
                                                     const gchar    *string);
gchar *             dax_repeat_count_to_string      (const DaxRepeatCount *count);

/*
 * DaxPreserveAspectRatio
 */

#define DAX_TYPE_PRESERVE_ASPECT_RATIO  (dax_preserve_aspect_ratio_get_type ())
#define DAX_VALUE_HOLDS_PRESERVE_ASPECT_RATIO \
    (G_VALUE_HOLDS ((x), DAX_TYPE_PRESERVE_ASPECT_RATIO))

#define DAX_PRESERVE_ASPECT_RATIO_ALIGN_DEFAULT \
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MID_Y_MID
#define DAX_PRESERVE_ASPECT_RATIO_ALIGN_CASE_SENSITIVE  1

typedef enum
{
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_NONE,
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MIN_Y_MIN,    /*< nick=xMinYMin >*/
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MID_Y_MIN,    /*< nick=xMidYMin >*/
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MAX_Y_MIN,    /*< nick=xMaxYMin >*/
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MIN_Y_MID,    /*< nick=xMinYMid >*/
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MID_Y_MID,    /*< nick=xMidYMid >*/
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MAX_Y_MID,    /*< nick=xMaxYMid >*/
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MIN_Y_MAX,    /*< nick=xMinYMax >*/
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MID_Y_MAX,    /*< nick=xMidYMax >*/
    DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MAX_Y_MAX,    /*< nick=xMaxYMax >*/
} DaxPreserveAspectRatioAlign;

typedef struct _DaxPreserveAspectRatio DaxPreserveAspectRatio;

#define DAX_PRESERVE_ASPECT_RATIO_FLAG_DEFER    1 << 0
#define DAX_PRESERVE_ASPECT_RATIO_FLAG_MEET     1 << 1
struct _DaxPreserveAspectRatio
{
    guint flags;
    DaxPreserveAspectRatioAlign align;
};

GType
dax_preserve_aspect_ratio_get_type (void) G_GNUC_CONST;

DaxPreserveAspectRatio *
dax_preserve_aspect_ratio_copy (const DaxPreserveAspectRatio *ar);
void
dax_preserve_aspect_ratio_free (DaxPreserveAspectRatio *ar);

void
dax_preserve_aspect_ratio_set_default (DaxPreserveAspectRatio *par);

gboolean
dax_preserve_aspect_ratio_has_defer (DaxPreserveAspectRatio *ar);
gboolean
dax_preserve_aspect_ratio_has_meet (DaxPreserveAspectRatio *ar);

gboolean
dax_preserve_aspect_ratio_from_string (DaxPreserveAspectRatio *ar,
                                       const gchar            *string);
gchar *
dax_preserve_aspect_ratio_to_string (DaxPreserveAspectRatio *ar);

G_END_DECLS

#endif /* __DAX_TYPES_H__ */
