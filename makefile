all: assembler

assembler: ./out/assembler.o ./out/macros_table.o ./out/preprocessor.o ./out/helpers.o ./out/symbol_table.o ./out/first_move.o ./out/second_move.o ./out/line_builders.o ./out/methods.o ./out/conversion.o
	gcc ./out/assembler.o ./out/macros_table.o ./out/preprocessor.o ./out/helpers.o ./out/symbol_table.o ./out/first_move.o ./out/second_move.o ./out/line_builders.o ./out/methods.o ./out/conversion.o -o assembler

./out/assembler.o: assembler.c
	gcc -Wall -ansi -pedantic -c assembler.c -o ./out/assembler.o

# ./helpers
./out/helpers.o: ./helpers/helpers.c
	gcc -Wall -ansi -pedantic -c ./helpers/helpers.c -o ./out/helpers.o

./out/line_builders.o: ./helpers/line_builders.c
	gcc -Wall -ansi -pedantic -c ./helpers/line_builders.c -o ./out/line_builders.o

# ./preprocessor
./out/preprocessor.o: ./preprocessor/preprocessor.c
	gcc -ansi -Wall -pedantic -c ./preprocessor/preprocessor.c -o ./out/preprocessor.o

./out/macros_table.o: ./preprocessor/macros_table.c
	gcc -ansi -Wall -pedantic -c ./preprocessor/macros_table.c -o ./out/macros_table.o
# ./symbols
./out/symbol_table.o: ./symbols/symbol_table.c
	gcc -ansi -Wall -pedantic -c ./symbols/symbol_table.c -o ./out/symbol_table.o
./out/first_move.o: ./symbols/first_move.c
	gcc -ansi -Wall -pedantic -c ./symbols/first_move.c -o ./out/first_move.o
./out/second_move.o: ./symbols/second_move.c
	gcc -ansi -Wall -pedantic -c ./symbols/second_move.c -o ./out/second_move.o

# ./methods
./out/methods.o: ./methods/methods.c
	gcc -Wall -ansi -pedantic -c ./methods/methods.c -o ./out/methods.o

# ./conversion
./out/conversion.o: ./conversion/conversion.c
	gcc -Wall -ansi -pedantic -c ./conversion/conversion.c -o ./out/conversion.o
