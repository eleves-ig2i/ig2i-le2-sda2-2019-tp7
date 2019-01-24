all : useHuffman.exe

useHuffman.exe : Huffman.o
	gcc Huffman.o useHuffman.c -o useHuffman.exe

useHuffman.o : Huffman.c
	gcc -c Huffman.c

clean :
	rm -rf *.o *.exe
