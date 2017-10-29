.PHONY: all clean samp-json

GPP = g++
OUTDIR = ./release
OUTFILE = $(OUTDIR)/json.so

COMPILE_FLAGS = -std=c++11 -c -fPIC -m32 -O3 -w -D LINUX -I ./amx/

all: samp-json

clean:
	-rm *.o
	-rm -r $(OUTDIR)

$(OUTDIR):
	mkdir -p $(OUTDIR)

json_spirit_reader.o:
	$(GPP) $(COMPILE_FLAGS) ./json_spirit/json_spirit_reader.cpp

json_spirit_writer.o:
	$(GPP) $(COMPILE_FLAGS) ./json_spirit/json_spirit_writer.cpp

json_spirit_value.o:
	$(GPP) $(COMPILE_FLAGS) ./json_spirit/json_spirit_value.cpp

samp-json: json_spirit_reader.o json_spirit_writer.o json_spirit_value.o $(OUTDIR)
	$(GPP) -O2 -fPIC -m32 -std=c++11 -fshort-wchar -shared -I . -o $(OUTFILE) *.o
