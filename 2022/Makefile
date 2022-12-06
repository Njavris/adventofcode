C++ := g++
BINS = $(subst /,/out,$(sort $(dir $(wildcard */))))

day%/out:
	$(C++) ./$(patsubst %/out,%,$@)/$(patsubst %/out,%.cpp,$@) -o ./$@

all: $(BINS)
	@echo $(DAYS)

clean:
	rm -rf $(BINS)
