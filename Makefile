#MAZUR DINU 313 CB
build: tema3
tema3: tema3.c tema3_functions.c
	gcc tema3.c tema3_functions.c -o tema3
run: tema3
	./tema3
clean:
	rm -f tema3