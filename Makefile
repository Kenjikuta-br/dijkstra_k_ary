# Variáveis
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -I./include
LDFLAGS = 
OBJDIR = obj
BINDIR = bin

# Fontes para o programa principal
MAIN_SOURCES = src/k_ary_heap.cpp src/dijkstra.cpp src/graph.cpp src/main.cpp
MAIN_OBJECTS = $(MAIN_SOURCES:src/%.cpp=$(OBJDIR)/%.o)
MAIN_TARGET = $(BINDIR)/dijkstra

# Fontes para o tester
TESTER_SOURCES = src/k_ary_heap.cpp src/dijkstra.cpp src/graph.cpp src/tester.cpp
TESTER_OBJECTS = $(TESTER_SOURCES:src/%.cpp=$(OBJDIR)/%.o)
TESTER_TARGET = $(BINDIR)/tester

# Diretórios
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Regra principal: compila ambos os programas
all: $(MAIN_TARGET) $(TESTER_TARGET)

# Regra para o programa principal
$(MAIN_TARGET): $(MAIN_OBJECTS)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJECTS) -o $(MAIN_TARGET) $(LDFLAGS)

# Regra para o tester
$(TESTER_TARGET): $(TESTER_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TESTER_OBJECTS) -o $(TESTER_TARGET) $(LDFLAGS)

# Regra para compilar os arquivos .cpp em arquivos .o
$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza de arquivos temporários
clean:
	rm -rf $(OBJDIR)/*.o $(MAIN_TARGET) $(TESTER_TARGET)

# Regras para rodar os programas
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

runtest: $(TESTER_TARGET)
	./$(TESTER_TARGET)

# Limpeza profunda (remove tudo)
distclean: clean
	rm -rf $(BINDIR)/*

.PHONY: clean distclean run runtest all