CXX     = g++
config	= debug
LDFLAGS = -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio

# Optimize if not in debug mode 
ifeq ($(config), release) 
	CFLAGS	= -c -Wall -std=gnu++0x -O2 
	OUTEXE  = bin/release/sfml-app
else  
	CFLAGS	= -c -Wall -std=gnu++0x  
	OUTEXE  = bin/debug/sfml-app
endif

SYMLINK  = run
SOURCES  = main.cpp 
OBJFILES = $(SOURCES:.cpp=.o)

all: $(SOURCES) $(OUTEXE)
		@ln -s $(OUTEXE) $(SYMLINK)

$(OUTEXE): $(OBJFILES)
	$(CXX) $(LDFLAGS) $(OBJFILES) -o $@

.cpp.o: 
	$(CXX) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o $(OUTEXE) ${SYMLINK}
