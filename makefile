all:
	gcc main.c lexer.c parser.c AST.c typeChecker.c symbolTable.c codegen.c  -o compiler

nasm:
	nasm -f elf64 *.asm
	gcc -no-pie -m64 *.o -o output
	./output

clean:
	rm -rf *.asm
	rm -rf *.o
	rm -rf compiler
	rm -rf token_stream.txt
	rm -rf without_comments.txt
