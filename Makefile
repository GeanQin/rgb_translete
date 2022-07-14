all:
	gcc image_trans.c main.c -o bmp888_to_argb

clean:
	rm -rf bmp888_to_argb