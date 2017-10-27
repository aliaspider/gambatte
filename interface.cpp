
#include "../interface.h"
#include "gambatte.h"
#include "inputgetter.h"

namespace gambatte {
static gambatte::GB gameboy;

class CustomInputGetter : public InputGetter {
public:
   pad_t* pad = NULL;
   unsigned operator()() {
      unsigned mask = 0;
      if (pad)
      {
         if(pad->buttons.A)
            mask |= A;
         if(pad->buttons.B)
            mask |= B;
         if(pad->buttons.start)
            mask |= START;
         if(pad->buttons.select)
            mask |= SELECT;
         if(pad->buttons.up)
            mask |= UP;
         if(pad->buttons.down)
            mask |= DOWN;
         if(pad->buttons.left)
            mask |= LEFT;
         if(pad->buttons.right)
            mask |= RIGHT;
      }
      return mask;
   }
};
static CustomInputGetter custom_input_getter;
}
void module_init(const module_init_info_t *init_info, module_info_t *module_info)
{
   if (gambatte::gameboy.load(init_info->filename) < 0)
      debug_log("load error\n");

   module_info->output_width = 160;
   module_info->output_height = 144;
   module_info->screen_format = screen_format_ARGB8888;
   module_info->stereo = true;
   module_info->framerate = 60;
   module_info->audio_rate = 35112 * module_info->framerate;

   gambatte::gameboy.setInputGetter(&gambatte::custom_input_getter);

   debug_log("module init\n");
}

void module_destroy()
{
   debug_log("module destroy\n");


}

void module_run(module_run_info_t *run_info)
{
   std::size_t max_samples = run_info->max_samples;
   gambatte::custom_input_getter.pad = run_info->pad;
   std::ptrdiff_t samples = gambatte::gameboy.runFor(run_info->screen.u32, run_info->pitch, run_info->sound_buffer.u32, max_samples);
   gambatte::custom_input_getter.pad = NULL;
   run_info->max_samples = max_samples;
   run_info->frame_completed = samples > 0;
}
