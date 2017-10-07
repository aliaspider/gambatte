
TARGET = gambatte_module.a
DEBUG = 0

platform = linux

BUILD_DIR = objs/$(platform)

ifeq ($(DEBUG),1)
   BUILD_DIR := $(BUILD_DIR)-dbg
endif

all: $(TARGET)

OBJS :=
OBJS += interface.o
OBJS += libgambatte/src/bitmap_font.o
OBJS += libgambatte/src/cpu.o
OBJS += libgambatte/src/gambatte.o
OBJS += libgambatte/src/initstate.o
OBJS += libgambatte/src/interrupter.o
OBJS += libgambatte/src/interruptrequester.o
OBJS += libgambatte/src/loadres.o
OBJS += libgambatte/src/memory.o
OBJS += libgambatte/src/sound.o
OBJS += libgambatte/src/state_osd_elements.o
OBJS += libgambatte/src/statesaver.o
OBJS += libgambatte/src/tima.o
OBJS += libgambatte/src/video.o
OBJS += libgambatte/src/mem/cartridge.o
OBJS += libgambatte/src/mem/memptrs.o
OBJS += libgambatte/src/mem/pakinfo.o
OBJS += libgambatte/src/mem/rtc.o
OBJS += libgambatte/src/sound/channel1.o
OBJS += libgambatte/src/sound/channel2.o
OBJS += libgambatte/src/sound/channel3.o
OBJS += libgambatte/src/sound/channel4.o
OBJS += libgambatte/src/sound/duty_unit.o
OBJS += libgambatte/src/sound/envelope_unit.o
OBJS += libgambatte/src/sound/length_counter.o
OBJS += libgambatte/src/video/ly_counter.o
OBJS += libgambatte/src/video/lyc_irq.o
OBJS += libgambatte/src/video/next_m0_time.o
OBJS += libgambatte/src/video/ppu.o
OBJS += libgambatte/src/video/sprite_mapper.o
OBJS += libgambatte/src/file/file.o

OBJS := $(addprefix $(BUILD_DIR)/,$(OBJS))

ifeq ($(DEBUG),1)
   FLAGS += -g -O0
else
   FLAGS += -O3
endif

FLAGS += -Wall -Wextra -Werror -Werror=implicit-function-declaration -Werror=incompatible-pointer-types
FLAGS += -Wno-array-bounds
FLAGS += -fomit-frame-pointer
FLAGS += -DHAVE_STDINT_H
FLAGS += -Ilibgambatte/src -Ilibgambatte/include -Icommon

CFLAGS += $(FLAGS)
CXXFLAGS += $(FLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/$(TARGET): $(OBJS) .lastbuild
	touch .lastbuild
	$(AR) rcs $@ $(OBJS)

$(TARGET): $(BUILD_DIR)/$(TARGET)
	cp $< $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $< $(CFLAGS) -MT $@ -MMD -MP -MF $(BUILD_DIR)/$*.depend -c -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $< $(CXXFLAGS) -MT $@ -MMD -MP -MF $(BUILD_DIR)/$*.depend -c -o $@

.lastbuild: ;

clean:
#	rm -rf objs
	rm -f $(OBJS) $(OBJS:.o=.depend)
	rm -f $(BUILD_DIR)/$(TARGET) $(TARGET) .lastbuild


-include $(OBJS:.o=.depend)
