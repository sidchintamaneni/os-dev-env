BASE_PROJ ?= $(shell pwd)

all: window window-surface

window: window.c
	gcc -g window.c -lSDL2 -o window

window-surface: w_surface.c
	gcc -g w_surface.c -lSDL2 -o w_surface

clean:
	rm -rf window w_surface

enter-docker:
	docker run -v ${BASE_PROJ}:/milo-stuff -w /milo-stuff -it random-runtime /bin/bash
