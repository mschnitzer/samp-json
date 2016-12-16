.PHONY: all clean samp-json

GPP = g++
OUTDIR = ./Release
OUTFILE = $(OUTDIR)/json.so

COMPILE_FLAGS = -lcurl -std=c++11 -c -fPIC -m32 -O3 -w -D LINUX -I ./amx/

all: samp-json

clean:
	-rm *.o
	-rm -r $(OUTDIR)

$(OUTDIR):
	mkdir -p $(OUTDIR)

samp-json: clean $(OUTDIR)
	$(GPP) $(COMPILE_FLAGS) *.cpp ./json_spirit/*.cpp
	$(GPP) -O2 -fPIC -m32 -std=c++11 -fshort-wchar -lcurl -shared -I . -I ./json_spirit -o $(OUTFILE) *.o
	-rm *.o
