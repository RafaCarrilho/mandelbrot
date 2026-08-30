.PHONY: clean
mandelbrot: main.c pthread1.c pthread2.c imagem.c openmp.c serial.c escape.c
	gcc main.c pthread1.c pthread2.c imagem.c openmp.c serial.c escape.c -o mandelbrot -lpthread -fopenmp -lm

clean:
	rm -f mandelbrot
