
#include "../interface.h"
#include "gambatte.h"
#include "inputgetter.h"

namespace gambatte
{
static gambatte::GB gameboy;

class CustomInputGetter : public InputGetter
{
public:
   pad_t *pad = NULL;
   typedef union
   {
      struct
      {
         int A : 1;
         int B : 1;
         int select : 1;
         int start : 1;
         int right : 1;
         int left : 1;
         int up : 1;
         int down : 1;
      };
      uint8_t mask;
   } gb_pad_t;
   unsigned operator()()
   {
      gb_pad_t gbpad;
      gbpad.A      = pad->buttons.A;
      gbpad.B      = pad->buttons.B;
      gbpad.select = pad->buttons.select;
      gbpad.start  = pad->buttons.start;
      gbpad.right  = pad->buttons.right;
      gbpad.left   = pad->buttons.left;
      gbpad.up     = pad->buttons.up;
      gbpad.down   = pad->buttons.down;
      return gbpad.mask;
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
   std::ptrdiff_t samples = gambatte::gameboy.runFor(run_info->screen.u32, run_info->pitch, run_info->sound_buffer.u32,
                            max_samples);
   run_info->max_samples = max_samples;
   run_info->frame_completed = samples > 0;
}
