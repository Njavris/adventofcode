C++ := g++
BINS = $(patsubst %,%out,$(sort $(dir $(wildcard */))))

day%/out:
	$(C++) ./$(patsubst %/out,%,$@)/$(patsubst %/out,%.cpp,$@) -o ./$@

all: $(BINS)
	@echo $(DAYS)

clean:
	rm -rf $(BINS)
