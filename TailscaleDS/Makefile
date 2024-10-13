#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

# Ensure DEVKITPRO and DEVKITARM are set
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# Project settings
TARGET      := TailscaleDS
BUILD       := build
SOURCES     := source
DATA        := data
INCLUDES    := include

APP_TITLE       := TailscaleDS
APP_DESCRIPTION := An app to connect to your Tailscale Network
APP_AUTHOR      := Jake Lindsey
ICON            := icon.png

# Use arm-none-eabi-gcc toolchain
CC      := $(DEVKITARM)/bin/arm-none-eabi-gcc
CXX     := $(DEVKITARM)/bin/arm-none-eabi-g++
LD      := $(DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY := $(DEVKITARM)/bin/arm-none-eabi-objcopy

# Options for code generation
ARCH        := -march=armv6k -mtune=mpcore -mfloat-abi=hard
CFLAGS      := -g -Wall -O2 -mword-relocations \
               -fomit-frame-pointer -ffast-math \
               $(ARCH) \
               -I$(DEVKITPRO)/libctru/include \
               -I$(DEVKITPRO)/citro2d/include \
               -I$(DEVKITPRO)/citro3d/include

CXXFLAGS    := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
ASFLAGS     := -g $(ARCH)
LDFLAGS     := -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(BUILD)/$(TARGET).map

# Link citro2d and citro3d
LIBS        := -lctru -lm -lcitro2d -lcitro3d

#---------------------------------------------------------------------------------
# Directories containing libraries (top level with include and lib)
#---------------------------------------------------------------------------------
LIBDIRS     := $(CTRULIB)

#---------------------------------------------------------------------------------
# No real need to edit below unless adding additional rules
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT := $(CURDIR)/$(BUILD)/$(TARGET)
export VPATH  := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
                 $(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES       := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES     := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES       := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

#---------------------------------------------------------------------------------
# Use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD := $(CC)
else
	export LD := $(CXX)
endif
#---------------------------------------------------------------------------------

export OFILES := $(addprefix $(BUILD)/, $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s:.o))

export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                  -I$(DEVKITPRO)/libctru/include \
                  $(foreach dir,$(LIBDIRS),-I$(dir)/include)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

ifeq ($(strip $(ICON)),)
	icons := $(wildcard *.png)
	ifneq (,$(findstring icon.png,$(icons)))
		export APP_ICON := $(TOPDIR)/icon.png
	endif
else
	export APP_ICON := $(TOPDIR)/$(ICON)
endif

ifeq ($(strip $(NO_SMDH)),)
	export _3DSXFLAGS += --smdh=$(CURDIR)/$(BUILD)/$(TARGET).smdh
endif

.PHONY: $(BUILD) clean all

all: $(BUILD)/$(TARGET).3dsx

$(BUILD):
	@[ -d $@ ] || mkdir -p $@

# Rule to compile .c files
$(BUILD)/%.o: %.c | $(BUILD)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile .cpp files
$(BUILD)/%.o: %.cpp | $(BUILD)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to generate the .elf file
$(BUILD)/$(TARGET).elf: $(OFILES)
	@echo "Linking $(TARGET).elf"
	@$(LD) $(OFILES) $(LDFLAGS) -o $@ $(LIBPATHS) $(LIBS)

# Rule to generate the .smdh file
$(BUILD)/$(TARGET).smdh: | $(BUILD)
	@echo "Building $(TARGET).smdh"
	@smdhtool --create "$(APP_TITLE)" "$(APP_DESCRIPTION)" "$(APP_AUTHOR)" "$(ICON)" $@

# Rule to generate the .3dsx file
$(BUILD)/$(TARGET).3dsx: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).smdh
	@echo "Building $(TARGET).3dsx"
	3dsxtool $< $(BUILD)/$(TARGET).3dsx --smdh=$(BUILD)/$(TARGET).smdh
	@echo "built ... $(BUILD)/$(TARGET).3dsx"
	@echo "built ... $(BUILD)/$(TARGET).smdh"

# Clean rule
clean:
	@echo "cleaning ..."
	@rm -rf $(BUILD)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
