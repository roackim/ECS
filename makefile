.PHONY: build test

build:
	meson setup build && meson compile -C build && meson install -C build

test: build
ifeq ($(OS),Windows_NT)
	.\bin\test_main.exe
else
	./bin/test_main.exe
endif