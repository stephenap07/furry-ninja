CXX     = g++ 
config	= debug
LDFLAGS = -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio

BIN_DIR = bin
BIN_NAME = sfml_app

# Optimize if not in debug mode 
ifeq ($(config), release) 
	CFLAGS	= -c -Wall -std=gnu++11 -O2
else  
	CFLAGS	= -c -Wall -std=gnu++11
endif

OUTEXE = $(BIN_DIR)/$(config)/$(BIN_NAME)

SRCDIR  := src
SRCEXT   = cpp

SYMLINK  = run
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJDIR   = obj
OBJFILES = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))

all: $(OBJDIR) $(OUTEXE) $(SYMLINK)

$(OBJDIR)/%.o: $(SOURCES) 
	$(CXX) $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OUTEXE): $(OBJFILES)
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_DIR)/$(config)
	$(CXX) $(LDFLAGS) $(OBJFILES) -o $@

$(SYMLINK):
	@ln -s $(OUTEXE) $(SYMLINK)

clean: 
	rm -rf *.o $(OUTEXE) ${SYMLINK} $(OBJDIR)
