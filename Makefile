CC=g++
CFLAGS=-lopengl32 -llibfreeglut -lglu32

all:
	$(CC) -o lightingtest lightingtest.cpp  $(CFLAGS)