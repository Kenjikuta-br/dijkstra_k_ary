# Variáveis
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -I./include
LDFLAGS = 
OBJDIR = obj
BINDIR = bin
SOURCES = src/k_ary_heap.cpp src/dijkstra.cpp
OBJECTS = $(SOURCES:src/%.cpp=obj/%.o)
TARGET = $(BINDIR)/dijkstra_program

# Diretórios
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Regra principal: compilar e linkar o programa final
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Regra para compilar os arquivos .cpp em arquivos .o
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza de arquivos temporários
clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/$(TARGET)

# Regra para rodar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para apenas compilar sem rodar
compile: $(OBJECTS)

# Limpeza profunda (remove tudo)
distclean: clean
	rm -rf $(BINDIR)/*

.PHONY: clean distclean run compile
