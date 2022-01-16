# ----------------------------------------------------------------
# environment
CXX	= g++

# ----------------------------------------------------------------
# options

CFLAGS	=  -fexceptions -g

# ----------------------------------------------------------------
# sources and objects
C_SRC	= Main.cpp Node.cpp Parser.cpp Dictionary.cpp
#C_OBJ	= $(C_SRC:.c=)
C_OBJ	= Main


# ----------------------------------------------------------------
# executables

EXEC		= $(C_OBJ) 

all:
#	$(C_OBJ):	$(C_SRC)
	$(CXX) -o $(C_OBJ)) $(CFLAGS) $(C_SRC)

# ----------------------------------------------------------------
# clean up

clean:
	/bin/rm -f $(EXEC) $(F_SRC:.f=.o)

# ----------------------------------------------------------------
# run

main:
#	$(C_OBJ):	$(C_SRC)
	$(CXX) -o $(C_OBJ) $(CFLAGS) $(C_SRC)


run:
#	$(CXX) -o $(EXEC) $(CFLAGS) $(C_SRC)
	./$(EXEC)

# ----------------------------------------------------------------
# End of Makefile
