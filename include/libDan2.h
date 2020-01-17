#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "audio/sound.h"
#include "audio/music.h"
#include "audio/resource_manager.h"
#include "audio/audio_channel.h"
#include "audio/real_audio_channel.h"
#include "audio/audio_controller.h"
#include "audio/audio_callback_interface.h"

#include "tools/sdl_tools.h"
#include "tools/log.h"
#include "tools/line.h"
#include "tools/polygon_2d.h"
#include "tools/box.h"
#include "tools/vector_2d.h"
#include "tools/point_2d.h"
#include "tools/tools.h"
#include "tools/sat_2d.h"
#include "tools/converters.h"

#include "input/sdl_input.h"

#include "video/sdl_video_tools.h"
#include "video/camera.h"
#include "video/resource_manager.h"
#include "video/screen.h"
#include "video/texture.h"
#include "video/ttf_font.h"
#include "video/surface.h"
#include "video/image.h"
#include "video/canvas.h"
#include "video/color.h"
#include "video/draw_info.h"
#include "video/rect.h"
#include "video/representation.h"
#include "video/raster_representation.h"
#include "video/bitmap_representation.h"
#include "video/ttf_representation.h"
#include "video/group_representation.h"
#include "video/primitive_representation.h"
#include "video/box/box_representation.h"
#include "video/line_representation.h"
#include "video/polygon_representation.h"
#include "video/point_representation.h"
