all: tb

tb: brightness.c
	gcc -Wall -O2 brightness.c -o tb

install: all
	@echo "Turning on setuid"
	sudo chown root tb
	sudo chmod 4711 tb
	sudo cp -f tb /home/stw/.local/bin/tb

uninstall:
	rm /home/stw/.local/bin/tb

clean:
	rm tb
