# Variáveis
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -I./include
LDFLAGS = 
OBJDIR = obj
BINDIR = bin

# Fontes para os programas
MAIN_SOURCES = src/k_ary_heap.cpp src/dijkstra.cpp src/graph.cpp src/main.cpp
MAIN_OBJECTS = $(MAIN_SOURCES:src/%.cpp=$(OBJDIR)/%.o)
MAIN_TARGET = $(BINDIR)/dijkstra

TESTER_SOURCES = src/k_ary_heap.cpp src/dijkstra.cpp src/graph.cpp src/tester.cpp
TESTER_OBJECTS = $(TESTER_SOURCES:src/%.cpp=$(OBJDIR)/%.o)
TESTER_TARGET = $(BINDIR)/tester

MEMORY_SOURCES = src/k_ary_heap.cpp src/dijkstra.cpp src/graph.cpp src/dijkstra_mem.cpp
MEMORY_OBJECTS = $(MEMORY_SOURCES:src/%.cpp=$(OBJDIR)/%.o)
MEMORY_TARGET = $(BINDIR)/dijkstra_mem

# Diretórios
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Regra principal: compila todos os programas
all: $(MAIN_TARGET) $(TESTER_TARGET) $(MEMORY_TARGET)

# Regras para os programas
$(MAIN_TARGET): $(MAIN_OBJECTS)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJECTS) -o $(MAIN_TARGET) $(LDFLAGS)

$(TESTER_TARGET): $(TESTER_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TESTER_OBJECTS) -o $(TESTER_TARGET) $(LDFLAGS)

$(MEMORY_TARGET): $(MEMORY_OBJECTS)
	$(CXX) $(CXXFLAGS) $(MEMORY_OBJECTS) -o $(MEMORY_TARGET) $(LDFLAGS)

# Regra para compilar os arquivos .cpp em arquivos .o
$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(OBJDIR)/*.o $(MAIN_TARGET) $(TESTER_TARGET) $(MEMORY_TARGET)

# Execução
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

runtest: $(TESTER_TARGET)
	./$(TESTER_TARGET)

runmem: $(MEMORY_TARGET)
	./$(MEMORY_TARGET)

# Limpeza completa
distclean: clean
	rm -rf $(BINDIR)/*

.PHONY: clean distclean run runtest runmem all