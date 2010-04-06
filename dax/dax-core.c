/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#include <string.h>

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-internals.h"
#include "dax-debug.h"
#include "dax-utils.h"
#include "dax-enum-types.h"
#include "dax-element-svg.h"
#include "dax-element-script.h"
#include "dax-core.h"

#define XML_NS_URI          "http://www.w3.org/XML/1998/namespace"
#define XMLNS_NS_URI        "http://www.w3.org/2000/xmlns/"
#define XML_EVENTS_NS_URI   "http://www.w3.org/2001/xml-events"
#define SVG_NS_URI          "http://www.w3.org/2000/svg"

const gchar *xml_ns;
const gchar *xmlns_ns;
const gchar *xmlevents_ns;
const gchar *svg_ns;

typedef struct {
    gint value;
    const gchar *name;
} enum_value;

static void
transform_string_float (const GValue *src,
                        GValue       *dest)
{
    gfloat value = 0.0f;
    gchar *str = (gchar *)g_value_get_string (src);

    _dax_utils_parse_simple_float (&str, &value);

    g_value_set_float (dest, value);
}

/* those two functions make some assumptions:
 *     - the enum values start at 0 and are valid until nb_values - 1
 *     - the preferred value is 0
 *     - the parsing is case insensitive */
static const char *
enum_find_name_by_value (enum_value *data,
                         gint        nb_values,
                         gint        value)
{
    if (value < 0 || value > nb_values)
        return NULL;

    return data[value].name;
}

static gint
enum_find_value_by_name (enum_value  *data,
                         gint         nb_values,
                         const gchar *name)
{
    gint i;

    for (i = 0; i < nb_values; i++)
        if (strcmp (name, data[i].name) == 0)
            return data[i].value;

    return 0;
}

#define DEFINE_TRANSFORM_FUNCS(enum_type, EnumType)                         \
static void                                                                 \
transform_dax_##enum_type##_string (const GValue *src,                   \
                                       GValue       *dest)                  \
{                                                                           \
    const gchar *name;                                                      \
    gint value;                                                             \
                                                                            \
    value = g_value_get_enum (dest);                                        \
    name = enum_find_name_by_value (enum_type##_enum_values,                \
                                    G_N_ELEMENTS (enum_type##_enum_values), \
                                    value);                                 \
                                                                            \
    if (name)                                                               \
        g_value_set_static_string (dest, name);                             \
    else                                                                    \
        g_value_set_static_string (dest,                                    \
                               "<invalid Dax" G_STRINGIFY(EnumType) ">");\
}                                                                           \
                                                                            \
static void                                                                 \
transform_string_dax_##enum_type (const GValue *src,                     \
                                     GValue       *dest)                    \
{                                                                           \
    const gchar *name;                                                      \
    gint value;                                                             \
                                                                            \
    name = g_value_get_string (src);                                        \
    value = enum_find_value_by_name (enum_type##_enum_values,               \
                                     G_N_ELEMENTS (enum_type##_enum_values),\
                                     name);                                 \
                                                                            \
    g_value_set_enum (dest, value);                                         \
}

#define INSTALL_TRANSFORM_FUNCS(enum_type, ENUM_TYPE)                       \
    g_value_register_transform_func (DAX_TYPE_##ENUM_TYPE,               \
                                     G_TYPE_STRING,                         \
                                     transform_dax_##enum_type##_string);\
    g_value_register_transform_func (G_TYPE_STRING,                         \
                                     DAX_TYPE_##ENUM_TYPE,               \
                                     transform_string_dax_##enum_type);

static enum_value svg_version_enum_values[3] = {
    { DAX_SVG_VERSION_1_0, "1.0" },
    { DAX_SVG_VERSION_1_1, "1.1" },
    { DAX_SVG_VERSION_1_2, "1.2" }
};

DEFINE_TRANSFORM_FUNCS(svg_version, SvgVersion)

static enum_value script_type_enum_values[2] = {
    { DAX_SCRIPT_TYPE_ECMASCRIPT,   "application/ecmascript"   },
    { DAX_SCRIPT_TYPE_JAVA_ARCHIVE, "application/java-archive" }
};

DEFINE_TRANSFORM_FUNCS(script_type, ScriptType)

void
dax_init (gint    *argc,
             gchar ***argv)
{
#ifdef DAX_ENABLE_DEBUG
    _dax_debug_init ();
#endif

    g_type_init ();
    clutter_init (argc, argv);

    xml_ns = I_(XML_NS_URI);
    xmlns_ns = I_(XMLNS_NS_URI);
    xmlevents_ns = I_(XML_EVENTS_NS_URI);
    svg_ns = I_(SVG_NS_URI);

    g_value_register_transform_func (G_TYPE_STRING,
                                     G_TYPE_FLOAT,
                                     transform_string_float);

    /* overrides of a few enums where we can't use the nick as the string to
     * be compared with the input (because the input is expected to have non
     * a-zA-Z- characters ("1.0", "application/ecmascript")) */
    INSTALL_TRANSFORM_FUNCS(svg_version, SVG_VERSION)
    INSTALL_TRANSFORM_FUNCS(script_type, SCRIPT_TYPE)
}
