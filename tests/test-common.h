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

const char image_use_base[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<svg xmlns=\"http://www.w3.org/2000/svg\" "
     "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
     "version=\"1.2\" baseProfile=\"tiny\" viewBox=\"0 0 100 100\">"
  "<g xml:base=\"http://a.example.org/aaa/\">\n"
    "<g xml:base=\"/bbb/ccc/\">\n"
      "<g xml:base=\"../ddd/\" xml:id=\"bar\">\n"
        "<image xml:id=\"foo\" xlink:href=\"foo.jpg\" width=\"100\" "
               "height=\"100\"/>\n"
       "</g>\n"
    "</g>\n"
  "</g>\n"
  "<g xml:base=\"http://z.example.net/zzz/\">\n"
    "<g xml:base=\"/yyy/xxx/\">\n"
       "<g xml:base=\"../xxx/\">\n"
         "<use xlink:href=\"#foo\" />\n"
         "<use xlink:href=\"#bar\" />\n"
         "<use xlink:href=\"#bar\" xml:base=\"../ggg/\" />\n"
       "</g>\n"
    "</g>\n"
  "</g>\n"
"</svg>";
