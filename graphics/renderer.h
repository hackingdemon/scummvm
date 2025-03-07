/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "common/scummsys.h"
#include "common/str.h"

namespace Graphics {

/**
 * @defgroup graphics_renderer Renderer
 * @ingroup graphics
 *
 * @brief API for managing renderer engines.
 *
 * @{
 */

/**
 * List of renderer types
 *
 * It specifies which rendering driver to use
 */
enum RendererType {
	kRendererTypeDefault = 0,
	kRendererTypeOpenGL = 1,
	kRendererTypeOpenGLShaders = 2,
	kRendererTypeTinyGL = 3
};

struct RendererTypeDescription {
	const char *code;
	const char *description;
	RendererType id;
};

const RendererTypeDescription *listRendererTypes();

/** Convert a renderer code to a RendererType enum value */
RendererType parseRendererTypeCode(const Common::String &code);

/** Get a character string code from a RendererType enum value */
Common::String getRendererTypeCode(RendererType type);

/** Get the best matching renderer among available renderers */
RendererType getBestMatchingAvailableRendererType(RendererType desired);
 /** @} */
} // End of namespace Graphics

#endif
