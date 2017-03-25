all:
	gcc main.c -o bin -Wl,--export-dynamic `pkg-config --libs --cflags gtk+-3.0 webkit2gtk-4.0`
run:
	LIBGL_DRI3_DISABLE=1 ./bin
install:
	rm -R ~/.local/muniSatipo
	mkdir ~/.local/muniSatipo
	cp bin ~/.local/muniSatipo/
	cp url_aceptados.txt ~/.local/muniSatipo/
	cp url_denegados.txt ~/.local/muniSatipo/
	cp gui2.glade ~/.local/muniSatipo/
	cp logo.png ~/.local/muniSatipo/
	cp muniSatipo.desktop ~/.local/share/applications/
