CXX ?= g++

CXXFLAGS =

OBJS = cannon.o

CFLAGS = -lopengl32 -llibfreeglut -lglu32

DEPS =

EXEC = cannon

OS1: build c
	
all: $(OBJS)
	$(CXX) -g $(OBJS) -o $(EXEC) $(CFLAGS) -Wall

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ -c $< -Wall

clean:
	$(RM) $(OBJS) $(EXEC)

c:
	$(RM) $(OBJS)
	$(RM) vgc*

e:
	$(RM) $(EXEC)
	$(RM) vgc*

run:
	./$(EXEC)