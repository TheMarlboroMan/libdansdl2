#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "lda/sound.h"
#include "lda/music.h"
#include "lda/resource_manager.h"
#include "lda/audio_channel.h"
#include "lda/real_audio_channel.h"
#include "lda/audio_controller.h"
#include "lda/audio_callback_interface.h"

#include "ldt/sdl_tools.h"
#include "ldt/log.h"
#include "ldt/line.h"
#include "ldt/polygon_2d.h"
#include "ldt/box.h"
#include "ldt/vector_2d.h"
#include "ldt/point_2d.h"
#include "ldt/tools.h"
#include "ldt/sat_2d.h"
#include "tools/converters.h"

#include "ldi/sdl_input.h"

#include "ldv/sdl_video_tools.h"
#include "ldv/camera.h"
#include "ldv/resource_manager.h"
#include "ldv/screen.h"
#include "ldv/texture.h"
#include "ldv/ttf_font.h"
#include "ldv/surface.h"
#include "ldv/image.h"
#include "ldv/canvas.h"
#include "ldv/color.h"
#include "ldv/draw_info.h"
#include "ldv/rect.h"
#include "ldv/representation.h"
#include "ldv/raster_representation.h"
#include "ldv/bitmap_representation.h"
#include "ldv/ttf_representation.h"
#include "ldv/group_representation.h"
#include "ldv/primitive_representation.h"
#include "ldv/box/box_representation.h"
#include "ldv/line_representation.h"
#include "ldv/polygon_representation.h"
#include "ldv/point_representation.h"
