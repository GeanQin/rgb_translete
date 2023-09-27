all:
	gcc bmp_trans.c main.c -o bmp_trans -lm

clean:
	rm -rf bmp_trans test*.bmp