CXX     = g++ 
config	= debug
LDFLAGS = -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio

# Optimize if not in debug mode 
ifeq ($(config), release) 
	CFLAGS	= -c -Wall -std=gnu++11 -O2
	OUTEXE  = bin/release/sfml-app
else  
	CFLAGS	= -c -Wall -std=gnu++11
	OUTEXE  = bin/debug/sfml-app
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
