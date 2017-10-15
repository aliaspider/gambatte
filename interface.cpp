
#include "../interface.h"
#include "gambatte.h"

gambatte::GB gameboy;

void module_init(const module_init_info_t *init_info, module_info_t *module_info)
{
   if (gameboy.load(init_info->filename) < 0)
      debug_log("load error\n");

   module_info->output_width = 160;
   module_info->output_height = 144;
   module_info->screen_format = screen_format_ARGB8888;
   module_info->stereo = true;
   module_info->framerate = 60;
   module_info->audio_rate = 35112 * module_info->framerate;

   debug_log("module init\n");
}

void module_destroy()
{
   debug_log("module destroy\n");


}

void module_run(module_run_info_t *run_info)
{
   std::size_t max_samples = run_info->max_samples;
   std::ptrdiff_t samples = gameboy.runFor(run_info->screen.u32, run_info->pitch, run_info->sound_buffer.u32, max_samples);
   run_info->max_samples = max_samples;
   run_info->frame_completed = samples > 0;
}
