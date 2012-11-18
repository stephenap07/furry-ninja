config	= debug
CXX     = g++ 
LDFLAGS = $(shell pkg-config --libs sfml-all)

CFLAGS = -c -Wall -std=gnu++11 

APP_NAME = sfml-app
OUTEXE  = bin/$(config)/sfml-app

# Add optimize flag if a release config
ifeq ($(config), release) 
	CFLAGS += -O2
endif

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
	mkdir $(OBJDIR)

$(OUTEXE): $(OBJFILES)
	$(CXX) $(LDFLAGS) $(OBJFILES) -o $@

$(SYMLINK):
	@ln -s $(OUTEXE) $(SYMLINK)

clean: 
	rm -rf *.o $(OUTEXE) ${SYMLINK} $(OBJDIR)
